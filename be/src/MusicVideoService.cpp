#include "MusicVideoService.h"

#include <future>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/regex.hpp>
#include <g3log/g3log.hpp>

#include "Album.h"
#include "BasicGenre.h"
#include "JobQueue.h"
#include "Song.h"
#include "MusicVideo.h"
#include "MusicManager.h"
#include "SoulSifterSettings.h"
#include "TagService.h"

using namespace std;

namespace dogatech {
namespace soulsifter {

JobQueue<std::vector<std::string>> MusicVideoService::job_queue;

std::string mv_yt_dlp_opts("--abort-on-error --compat-options filename --restrict-filenames --cookies-from-browser chrome --write-thumbnail ");
std::string yt_dlp_opts("--abort-on-error --compat-options filename --restrict-filenames --cookies-from-browser chrome --print-json --write-thumbnail --restrict-filenames --extract-audio --audio-format mp3 --audio-quality 0 --quiet --download-archive /tmp/ss-ytdl.txt ");

namespace {

string removeSpecialCharsFromPath(string filepath) {
  string newPath = MusicManager::cleanDirName(filepath);
  if (!newPath.compare(filepath)) return filepath;

  LOG(INFO) << "Renaming '" << filepath << "'' to '" << newPath << "'";
  try {
      boost::filesystem::path src(filepath);
      boost::filesystem::path dest(newPath);
      boost::filesystem::rename(src, dest);
      return newPath;
  } catch (const boost::filesystem::filesystem_error& ex) {
      LOG(WARNING) << "Unable to rename music video related file '" << filepath << "'\n" << ex.what();
      return filepath;
  }
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
    return !std::isspace(ch);
  }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

// returns true if both characters are a space
template<char C>
bool bothAre(char lhs, char rhs) { return (lhs == rhs) && (lhs == C); }

}  // namespace

future<vector<string>> MusicVideoService::downloadAudioAsync(const string& url) {
  return job_queue.push([url]() { return MusicVideoService::downloadAudio(url); });
}

vector<string> MusicVideoService::downloadAudio(const string& url) {
  LOG(INFO) << "downloadYouTubeAudio with url " << url;
  vector<string> filepaths;

  boost::filesystem::path tmpPath(SoulSifterSettings::getInstance().get<string>("dir.tmp"));  // todo: use os.tmpdir()
  if (!boost::filesystem::exists(tmpPath)) {
    if (!boost::filesystem::create_directories(tmpPath)) {
      LOG(WARNING) << "Unable to create temporary directory " << tmpPath;
      return filepaths;
    }
  } else if (!boost::filesystem::is_directory(tmpPath)) {
    LOG(WARNING) << "Temporary directory is not a directory " << tmpPath;
    return filepaths;
  }

  FILE *fpipe;
  stringstream command;
  command << "cd " << tmpPath << "; yt-dlp " << yt_dlp_opts << url;
  if (!(fpipe = (FILE*)popen(command.str().c_str(), "r"))) {
    LOG(WARNING) << "Problem with yt-dlp pipe.";
    return filepaths;
  }

  char buffer[1024];
  stringstream ss;
  LOG(INFO) << "Running command '" << command.str() << "'";
  while (fgets(buffer, sizeof buffer, fpipe)) {
    ss << buffer;
  }

  pclose(fpipe);

  // read output
  string json;
  bool isCoverAdded = false;  // in case downloading full album
  while (std::getline(ss, json, '\n')) {
    if (json.at(0) == '{') {
      boost::property_tree::ptree ptree;
      std::stringstream tmp(json);
      read_json(tmp, ptree);
      if (SoulSifterSettings::getInstance().get<bool>("app.debug")) {
        LOG(DEBUG) << "song info";
        LOG(DEBUG) << json;
      }

      Song* song = new Song();
      Album* album = new Album();
      song->setAlbum(album);

      string baseFileName = ptree.get<string>("_filename").substr(0, ptree.get<string>("_filename").size() - ptree.get<string>("ext").size());
      song->setFilepath(SoulSifterSettings::getInstance().get<string>("dir.tmp") + '/' + baseFileName + "mp3");
      album->setCoverFilepath(SoulSifterSettings::getInstance().get<string>("dir.tmp") + '/' + baseFileName + "jpg");
      if (!boost::filesystem::exists(album->getCoverFilepath())) {
        album->setCoverFilepath(SoulSifterSettings::getInstance().get<string>("dir.tmp") + '/' + baseFileName + "webp");
      }
      song->setLowQuality(true);
      string date;
      if (url.find("music.youtube") != std::string::npos) {
        // youtube music
        song->setTitle(ptree.get<string>("track"));
        LOG(DEBUG) << "artist = " << ptree.get<string>("artist");
        LOG(DEBUG) << "creator = " << ptree.get<string>("creator");
        LOG(DEBUG) << "channel = " << ptree.get<string>("channel");
        // youtube music adds featuring and remixers to artists, so we remove it possibly here
        {
          std::vector<std::string> artists;
          boost::split(artists, ptree.get<string>("artist"), boost::is_any_of(","));
          // remove remixer
          for (size_t i = 1; i < artists.size(); ++i) {
            if (song->getTitle().find(trim_copy(artists[i])) == std::string::npos) {
              artists[0] += ", " + artists[i];
            }
          }
          song->setArtist(artists[0]);
          // update featuring
          if (song->getArtist().rfind(ptree.get<string>("channel"), 0) == 0
              && ptree.get<string>("artist") != ptree.get<string>("channel")) {
            song->setArtist(ptree.get<string>("channel") + " (ft. " + song->getArtist().substr(ptree.get<string>("channel").length() + 2) + ")");
          }
          // consolidate double spaces to a single space
          string s = song->getArtist();
          std::string::iterator new_end = std::unique(s.begin(), s.end(), bothAre<' '>);
          s.erase(new_end, s.end());
          song->setArtist(s);
        }
        album->setName(ptree.get<string>("album"));
        song->setTrack(ptree.get<string>("playlist_index"));
        date = ptree.get<string>("release_date", "00000000");
        if (!date.compare("null") || !date.compare("00000000")) {
          string songDescription = ptree.get<string>("description");
          boost::regex descReleasedRegex("Released on: (\\d{4})-(\\d{2})-(\\d{2})\\b");
          boost::smatch match;
          if (boost::regex_search(songDescription, match, descReleasedRegex, boost::match_extra)) {
            date = match[1] + match[2] + match[3];
          }
        }
        if (!date.compare("null") || !date.compare("00000000")) {
          int yr = ptree.get<int>("release_year", 0);
          if (yr > 0) date = std::to_string(yr) + "0000";
        }
      } else {
        // youtube
        song->setYoutubeId(ptree.get<string>("id"));
        song->setCurator(ptree.get<string>("uploader"));
        string title = ptree.get<string>("title");
        if (!MusicManager::getInstance().splitArtistAndTitle(title, song)) {
          song->setTitle(title);
          song->setArtist(ptree.get<string>("uploader"));
          // Remove " - Topic" from artist field
          boost::regex artistTopicRegex(" - Topic$");
          song->setArtist(boost::regex_replace(song->getArtist(), artistTopicRegex, ""));
        }
        date = ptree.get<string>("upload_date", "00000000");
      }
      if (!date.empty() && !!date.compare("null")) {
        album->setReleaseDateYear(std::stoi(date.substr(0, 4)));
        album->setReleaseDateMonth(std::stoi(date.substr(4, 2)));
        album->setReleaseDateDay(std::stoi(date.substr(6, 4)));
      }

      TagService::writeId3v2Tag(song);
      filepaths.push_back(song->getFilepath());
      if (url.find("music.youtube") == std::string::npos || !isCoverAdded) {
        filepaths.push_back(album->getCoverFilepath());
        isCoverAdded = true;
      }
      delete song;
    }
  }
  return filepaths;
}

MusicVideo* MusicVideoService::associateYouTubeVideo(Song* song, const string& url) {
  LOG(INFO) << "Associate YouTube video " << url << " with song " << song->getId();

  boost::filesystem::path mvBasePath(SoulSifterSettings::getInstance().get<string>("dir.mv"));
  if (!boost::filesystem::exists(mvBasePath) || !boost::filesystem::is_directory(mvBasePath)) {
    LOG(WARNING) << "Music video base path does not exist. " << mvBasePath;
    return NULL;
  }

  string mvArtistDir;
  {
    stringstream ss;
    ss << mvBasePath.string() << MusicManager::cleanDirName(song->getAlbum()->getBasicGenre()->getName())
       << "/" << MusicManager::cleanDirName(song->getArtist());
    mvArtistDir = ss.str();
  }
  boost::filesystem::path mvArtistPath(mvArtistDir);
  if (!boost::filesystem::exists(mvArtistPath)) {
    if (!boost::filesystem::create_directories(mvArtistPath)) {
      LOG(WARNING) << "Unable to create music video artist directory " << mvArtistDir;
      return NULL;
    }
  } else if (!boost::filesystem::is_directory(mvArtistPath)) {
    LOG(WARNING) << "Music video artist directory is not a directory " << mvArtistDir;
    return NULL;
  }

  FILE *fpipe;
  stringstream command;
  command << "cd \"" << mvArtistDir << "\"; yt-dlp " << mv_yt_dlp_opts << url;
  if (!(fpipe = (FILE*)popen(command.str().c_str(), "r"))) {
    LOG(WARNING) << "Problem with yt-dlp pipe.";
    pclose(fpipe);
    return NULL;
  }

  char buffer[1024];
  stringstream ss;
  LOG(INFO) << "Command output for '" << command.str() << "':";
  while (fgets(buffer, sizeof buffer, fpipe)) {
    ss << buffer;
  }

  pclose(fpipe);

  string output(ss.str());
  LOG(INFO) << output;
  boost::regex thumbnailRegex("Writing thumbnail to: (.*)$");
  boost::regex mp4VideoRegex("Merging formats into \"(.*)mp4\"$");
  boost::regex mkvVideoRegex("Merging formats into \"(.*)mkv\"$");
  boost::smatch match;
  string tmpVideoName;
  bool needsConversion;
  MusicVideo* musicVideo = new MusicVideo();
  vector<string> lines;
  boost::split(lines, output, boost::is_any_of("\n"));
  for (string line : lines) {
    if (boost::regex_search(line, match, thumbnailRegex)) {
      stringstream ssTn;
      ssTn << mvArtistDir << "/" << match[1];
      musicVideo->setThumbnailFilePath(ssTn.str());
    } else if (boost::regex_search(line, match, mkvVideoRegex)) {
      tmpVideoName = match[1];
      needsConversion = true;
    } else if (boost::regex_search(line, match, mp4VideoRegex)) {
      stringstream ss;
      ss << mvArtistDir << "/" << match[1] << "mp4";
      musicVideo->setFilePath(ss.str());
      needsConversion = false;
    }
  }
  if (tmpVideoName.empty() && musicVideo->getFilePath().empty()) {
    delete musicVideo;
    LOG(WARNING) << "Did not find music video file from yt-dlp output.";
    return NULL;
  }

  // Convert to mp4 from mkv (mostly need audio codec conversion)
  if (needsConversion) {
    stringstream ffmpegCmd;
    ffmpegCmd << "cd \"" << mvArtistDir << "\"; ffmpeg -i '" << tmpVideoName << "mkv' -c:v copy -c:a aac '" << tmpVideoName << "mp4' && rm '" << tmpVideoName << "mkv'";
    if (!(fpipe = (FILE*)popen(ffmpegCmd.str().c_str(), "r"))) {
      LOG(WARNING) << "Problem with ffmpeg pipe.";
      pclose(fpipe);
      return NULL;
    }
    pclose(fpipe);

    // update music video
    stringstream ssMv;
    ssMv << mvArtistDir << "/" << tmpVideoName << "mp4";
    musicVideo->setFilePath(ssMv.str());
  }

  // remove special chars from files just in case
  // probably don't need with --restrict-filenames, but whatever
  musicVideo->setFilePath(removeSpecialCharsFromPath(musicVideo->getFilePath()));
  musicVideo->setThumbnailFilePath(removeSpecialCharsFromPath(musicVideo->getThumbnailFilePath()));

  // remove base path
  musicVideo->setFilePath(boost::algorithm::ireplace_first_copy(musicVideo->getFilePath(), SoulSifterSettings::getInstance().get<string>("dir.mv"), ""));
  musicVideo->setThumbnailFilePath(boost::algorithm::ireplace_first_copy(musicVideo->getThumbnailFilePath(), SoulSifterSettings::getInstance().get<string>("dir.mv"), ""));

  musicVideo->save();

  song->setMusicVideoId(musicVideo->getId());
  song->update();

  return musicVideo;
}

}  // namespace soulsifter
}  // namespace dogatech

