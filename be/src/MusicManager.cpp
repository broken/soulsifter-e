//
//  MusicManager.m
//  soul-sifter
//
//  Created by Robby Neale on 4/28/12.
//  Copyright (c) 2012 Dogatech. All rights reserved.
//
//  Core that manages possible tags for the music file. This is not multi-thread safe.
//
//  At some point I'd like to upgrade this to C++11, but it'll require getting libs compiled
//  for it as well. In cases I used homebrew, update the formula to include the C++11 compiler:
//  ENV['CXXFLAGS'] = " -std=c++11 -stdlib=libc++"

#include "MusicManager.h"

#include <algorithm>
#include <ctype.h>
#include <map>
#include <regex.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <g3log/g3log.hpp>
#include <taglib/apetag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mp4file.h>
#include <taglib/mpegfile.h>
#include <taglib/popularimeterframe.h>
#include <taglib/tbytevector.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tpropertymap.h>
#include <taglib/tstring.h>
#include <taglib/tstringlist.h>

#include "Album.h"
#include "AlbumPart.h"
#include "BasicGenre.h"
#include "MysqlAccess.h"
#include "Song.h"
#include "SoulSifterSettings.h"
#include "Style.h"
#include "TagService.h"

using namespace std;

namespace dogatech {
namespace soulsifter {

# pragma mark private helpers

namespace {

  const string FEATURING_REGEX = " [(]?[Ff](eaturing|t[.]?|eat[.]?) ([^()]+)[)]?";
  const string REMIX_TERM_REGEX = "[Rr]emix|[Rr]mx|[Mm]ix|[Rr]efix|[Dd]ub|[Ff]lip|[Bb]ootleg|[Ee]dit|[Cc]ut|[Vv]ocal";
  const string REMIX_REGEX = "[(]([^()]+) (" + REMIX_TERM_REGEX + ")[)]";
  const string NON_REMIXER_REGEX = "([Oo]riginal|[Rr]adio|[Vv]ideo|" + REMIX_TERM_REGEX + ")$";
  const string POSSESSION_REGEX = "^[^']'";

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

void moveFeaturingFromTitleToArtist(Song* updatedSong) {
  boost::regex featRegex(FEATURING_REGEX);
  boost::smatch featMatch;
  if (boost::regex_search(updatedSong->getTitle(), featMatch, featRegex, boost::match_extra)) {
    updatedSong->setArtist(updatedSong->getArtist() + " (ft. " + featMatch[2] + ")");
    updatedSong->setTitle(boost::regex_replace(updatedSong->getTitle(), featRegex, ""));
  }
}

void updateFeaturingInArtist(Song* updatedSong) {
  boost::regex featRegex(FEATURING_REGEX);
  boost::smatch featMatch;
  if (boost::regex_search(updatedSong->getArtist(), featMatch, featRegex, boost::match_extra)) {
    updatedSong->setArtist(boost::regex_replace(updatedSong->getArtist(), featRegex, "") + " (ft. " + featMatch[2] + ")");
  }
}

void removeFeaturingFromAlbumArtist(Song* updatedSong) {
  boost::regex featRegex(FEATURING_REGEX);
  boost::smatch featMatch;
  if (boost::regex_search(updatedSong->getAlbum()->getArtist(), featMatch, featRegex, boost::match_extra)) {
    updatedSong->getAlbum()->setArtist(boost::regex_replace(updatedSong->getAlbum()->getArtist(), featRegex, ""));
  }
}

void copyRemixer(Song* updatedSong) {
  boost::regex rmxrRegex(REMIX_REGEX);
  boost::smatch rmxrMatch;
  if (boost::regex_search(updatedSong->getTitle(), rmxrMatch, rmxrRegex, boost::match_extra) &&
      updatedSong->getRemixer().length() == 0) {
    string remixer(rmxrMatch[1]);
    // Remove possession "'s" from remixer name
    size_t apostropheIndex = remixer.find("'");
    if (apostropheIndex != std::string::npos) remixer = remixer.substr(0, apostropheIndex);
    // Remove dangling remix terms that shouldn't be a part of the name
    rtrim(remixer);
    boost::regex nonRemixerRegex(NON_REMIXER_REGEX);
    remixer = boost::regex_replace(remixer, nonRemixerRegex, "");
    trim(remixer);
    updatedSong->setRemixer(remixer);
  }
}

void guessGenresForSong(Song* song) {
  // Search by Remixer on artist, album artist, & remixer fields
  if (!song->getRemixer().empty()) {
    for (int i = 0; i < 2; ++i) {
      try {
        LOG(DEBUG) << "--------------------------------";
        LOG(DEBUG) << song->getRemixer();
        sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select styleId from songstyles st inner join (select genres, id, count(*) cnt from (select group_concat(styleid) as genres, s.id from songstyles ss inner join songs s on ss.songid = s.id inner join albums a on s.albumid = a.id where a.artist = ? or s.artist = ? or s.remixer = ? group by ss.songid) j group by genres order by cnt desc limit 1) i on st.songid=i.id");
        ps->setString(1, song->getRemixer());
        ps->setString(2, song->getRemixer());
        ps->setString(3, song->getRemixer());
        sql::ResultSet *rs = ps->executeQuery();
        vector<int> styleIds;
        if (rs->next()) {
          styleIds.push_back(rs->getInt("styleId"));
        }
        rs->close();
        delete rs;
        if (styleIds.size()) {
          song->setStyleIds(styleIds);
          return;
        }
      } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        bool reconnected = MysqlAccess::getInstance().reconnect();
        LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
      }
    }
  }
  // Search by Curator
  if (!song->getCurator().empty()) {
    for (int i = 0; i < 2; ++i) {
      try {
        LOG(DEBUG) << "--------------------------------";
        LOG(DEBUG) << song->getCurator();
        sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select styleId from songstyles st inner join (select genres, id, count(*) cnt from (select group_concat(styleid) as genres, s.id from songstyles ss inner join songs s on ss.songid = s.id where s.curator = ? group by ss.songid) j group by genres order by cnt desc limit 1) i on st.songid=i.id");
        ps->setString(1, song->getCurator());
        sql::ResultSet *rs = ps->executeQuery();
        vector<int> styleIds;
        if (rs->next()) {
          styleIds.push_back(rs->getInt("styleId"));
        }
        rs->close();
        delete rs;
        if (styleIds.size()) {
          song->setStyleIds(styleIds);
          return;
        }
      } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        bool reconnected = MysqlAccess::getInstance().reconnect();
        LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
      }
    }
  }
  // Search by Album Artist on artist, album artist, & remixer fields
  if (!song->getAlbum()->getArtist().empty()) {
    for (int i = 0; i < 2; ++i) {
      try {
        LOG(DEBUG) << "--------------------------------";
        LOG(DEBUG) << song->getAlbum()->getArtist();
        sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select styleId from songstyles st inner join (select genres, id, count(*) cnt from (select group_concat(styleid) as genres, s.id from songstyles ss inner join songs s on ss.songid = s.id inner join albums a on s.albumid = a.id where a.artist = ? or s.artist = ? or s.remixer = ? group by ss.songid) j group by genres order by cnt desc limit 1) i on st.songid=i.id");
        ps->setString(1, song->getAlbum()->getArtist());
        ps->setString(2, song->getAlbum()->getArtist());
        ps->setString(3, song->getAlbum()->getArtist());
        sql::ResultSet *rs = ps->executeQuery();
        vector<int> styleIds;
        if (rs->next()) {
          styleIds.push_back(rs->getInt("styleId"));
        }
        rs->close();
        delete rs;
        if (styleIds.size()) {
          song->setStyleIds(styleIds);
          return;
        }
      } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        bool reconnected = MysqlAccess::getInstance().reconnect();
        LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
      }
    }
  }
}

}  // namespace

# pragma mark initialization

MusicManager::MusicManager() :
lastParsedSong(NULL),
lastSongFixed(NULL),
artistToGenre(1600),
albumSubPathForImage() {
}

MusicManager::~MusicManager() {
    // TODO delete artistToGenre
}

# pragma mark tagging

void MusicManager::readTagsFromSong(Song* song) {
    if (!boost::algorithm::iends_with(song->getFilepath(), ".mp3") &&
        !boost::algorithm::iends_with(song->getFilepath(), ".mp4")) {
      LOG(WARNING) << "can only read id3v2 tags from mpeg files: " << song->getFilepath();
      return;
    }
    if (!song->getAlbum()) {
        Album album;
        song->setAlbum(album);
    }

    string songFilepath = song->getFilepath();
    if (!boost::filesystem::exists(songFilepath)) {
      songFilepath = SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath();
      if (!boost::filesystem::exists(songFilepath)) {
        LOG(WARNING) << "unable to find song: " << song->getFilepath();
      }
    }

    if (boost::algorithm::iends_with(song->getFilepath(), ".mp3")) {
        TagLib::MPEG::File f(songFilepath.c_str());
        TagLib::ID3v1::Tag* id3v1 = f.ID3v1Tag();
        if (id3v1) {
            stringstream ss;
            if (id3v1->title() != TagLib::String::null) song->setTitle(trim_copy(id3v1->title().to8Bit()));
            if (id3v1->artist() != TagLib::String::null) song->setArtist(trim_copy(id3v1->artist().to8Bit()));
            if (id3v1->album() != TagLib::String::null) song->getAlbum()->setName(trim_copy(id3v1->album().to8Bit()));
            if (id3v1->comment() != TagLib::String::null) song->setComments(trim_copy(id3v1->comment().to8Bit()));
            //TODO if (id3v1->genre() != TagLib::String::null) song->setGenre(id3v1->genre().to8Bit());
            if (id3v1->year() != 0) song->getAlbum()->setReleaseDateYear(id3v1->year());
            if (id3v1->track() != 0) {
                ss.clear();
                ss << id3v1->track();
                song->setTrack(ss.str());
            }
        }
        TagService::readId3v2Tag(song);
    } else if (boost::algorithm::iends_with(song->getFilepath(), ".m4a") ||
               boost::algorithm::iends_with(song->getFilepath(), ".mp4") ||
               boost::algorithm::iends_with(song->getFilepath(), ".aac") ||
               boost::algorithm::iends_with(song->getFilepath(), ".alac")) {
      TagLib::MP4::File f(songFilepath.c_str());
      TagLib::MP4::Tag* tag = f.tag();
      if (tag) {
        stringstream ss;
        if (tag->title() != TagLib::String::null) song->setTitle(trim_copy(tag->title().to8Bit()));
        if (tag->artist() != TagLib::String::null) song->setArtist(trim_copy(tag->artist().to8Bit()));
        if (tag->track() != 0) song->setTrack(trim_copy((ostringstream() << tag->track()).str()));
        if (tag->album() != TagLib::String::null) song->getAlbum()->setName(trim_copy(tag->album().to8Bit()));
        if (tag->comment() != TagLib::String::null) song->setComments(trim_copy(tag->comment().to8Bit()));
        if (tag->year() != 0) song->getAlbum()->setReleaseDateYear(tag->year());
      }
      TagService::readId3v2Tag(song);
    }
}

void MusicManager::writeTagsToSong(Song* song) {
  delete lastSongFixed;
  lastSongFixed = new Song(*song);

  TagService::writeId3v2Tag(song);
}

# pragma mark monitor changes

  void MusicManager::updateSongWithChanges(const Song& song, Song* updatedSong) {
    *updatedSong = song;

    // compare with last
    if (lastParsedSong && lastSongFixed) {
      if (!song.getArtist().compare(lastParsedSong->getArtist())) {
        // featuring normally comes from title field rather than artist,
        // so we remove it since it rarely matches in practice.
        boost::regex featRegex(" [(]ft[.] .*");
        updatedSong->setArtist(boost::regex_replace(lastSongFixed->getArtist(), featRegex, ""));
      } else {
        // youtube music adds featuring and remixers to artists, so we remove it possibly here
        std::vector<std::string> artists;
        boost::split(artists, updatedSong->getArtist(), boost::is_any_of(","));
        for (size_t i = 1; i < artists.size(); ++i) {
          if (updatedSong->getTitle().find(trim_copy(artists[i])) == std::string::npos) {
            artists[0] += ", " + artists[i];
          }
        }
      }
      if (song.getTrack().length() == 0) {
        int trackNum = atoi(lastSongFixed->getTrack().c_str());  // returns 0 on error
        if (trackNum > 0) {
          ++trackNum;
          if (lastSongFixed->getTrack().length() > 1 && trackNum < 10) {
            updatedSong->setTrack('0' + (ostringstream() << trackNum).str());
          } else {
            updatedSong->setTrack((ostringstream() << trackNum).str());
          }
        }
      }
      // I created these 'Const' methods so you can retrieve model (if needed) without saving the pointer and method remains const
      Album* songAlbum = song.getAlbumConst();
      AlbumPart* songAlbumPart = song.getAlbumPartConst();
      // we shouldn't auto set track title b/c it changes so much
      // nor should we auto update remixer
      if (!songAlbum->getArtist().compare(lastParsedSong->getAlbum()->getArtist())) {
        updatedSong->getAlbum()->setArtist(lastSongFixed->getAlbum()->getArtist());
      }
      if (!songAlbum->getName().compare(lastParsedSong->getAlbum()->getName())) {
        updatedSong->getAlbum()->setName(lastSongFixed->getAlbum()->getName());
      }
      updatedSong->getAlbum()->setMixed(lastSongFixed->getAlbum()->getMixed());
      if (!songAlbum->getLabel().compare(lastParsedSong->getAlbum()->getLabel())) {
        updatedSong->getAlbum()->setLabel(lastSongFixed->getAlbum()->getLabel());
      }
      if (!songAlbum->getCatalogId().compare(lastParsedSong->getAlbum()->getCatalogId())) {
        updatedSong->getAlbum()->setCatalogId(lastSongFixed->getAlbum()->getCatalogId());
      }
      if (songAlbum->getReleaseDateYear() == lastParsedSong->getAlbum()->getReleaseDateYear()) {
        updatedSong->getAlbum()->setReleaseDateYear(lastSongFixed->getAlbum()->getReleaseDateYear());
      }
      if (songAlbum->getReleaseDateMonth() == lastParsedSong->getAlbum()->getReleaseDateMonth()) {
        updatedSong->getAlbum()->setReleaseDateMonth(lastSongFixed->getAlbum()->getReleaseDateMonth());
      }
      if (songAlbum->getReleaseDateDay() == lastParsedSong->getAlbum()->getReleaseDateDay()) {
        updatedSong->getAlbum()->setReleaseDateDay(lastSongFixed->getAlbum()->getReleaseDateDay());
      }
      if (songAlbum->getBasicGenreId() == lastParsedSong->getAlbum()->getBasicGenreId()) {
        updatedSong->getAlbum()->setBasicGenreId(lastSongFixed->getAlbum()->getBasicGenreId());
      }
      if (song.getComments().size() > 0 && !song.getComments().compare(lastParsedSong->getComments())) {
        updatedSong->setComments(lastSongFixed->getComments());
      }
      // we may not always have an album part
      if (songAlbumPart && lastParsedSong->getAlbumPart()) {
        if (!songAlbumPart->getName().compare(lastParsedSong->getAlbumPart()->getName()) &&
            !songAlbumPart->getPos().compare(lastParsedSong->getAlbumPart()->getPos())) {
          if (lastSongFixed->getAlbumPart()) {
            updatedSong->getAlbumPart()->setName(lastSongFixed->getAlbumPart()->getName());
            updatedSong->getAlbumPart()->setPos(lastSongFixed->getAlbumPart()->getPos());
          } else {
            updatedSong->setAlbumPartId(0);
          }
        } else if (!songAlbumPart->getName().compare(lastParsedSong->getAlbumPart()->getName()) &&
                   lastSongFixed->getAlbumPart()) {
          updatedSong->getAlbumPart()->setName(lastSongFixed->getAlbumPart()->getName());
        } else if (!songAlbumPart->getPos().compare(lastParsedSong->getAlbumPart()->getPos()) &&
                   lastSongFixed->getAlbumPart()) {
          updatedSong->getAlbumPart()->setPos(lastSongFixed->getAlbumPart()->getPos());
        }
      }
    }

    // copy remixer
    copyRemixer(updatedSong);

    // add an album artist if one does not exist & not a compilation
    if (updatedSong->getAlbum()->getArtist().empty()
        && !(lastSongFixed && lastSongFixed->getAlbum()->getArtist().empty() &&
             lastParsedSong && !song.getAlbumConst()->getName().compare(lastParsedSong->getAlbum()->getName()))) {
      updatedSong->getAlbum()->setArtist(updatedSong->getArtist());
    }
    // add an album name if one does not exist
    if (updatedSong->getAlbum()->getName().empty()) {
      updatedSong->getAlbum()->setName(updatedSong->getTitle());
    }
    // remove album part if there is just one
    if (updatedSong->getAlbumPart() && !updatedSong->getAlbumPart()->getPos().compare("1/1")) {
      updatedSong->getAlbumPart()->setPos("");
    }

    // handle featuring
    updateFeaturingInArtist(updatedSong);
    moveFeaturingFromTitleToArtist(updatedSong);
    removeFeaturingFromAlbumArtist(updatedSong);

    // guess genres
    guessGenresForSong(updatedSong);

    delete lastParsedSong;
    lastParsedSong = new Song(song);
  }

    void MusicManager::setNewSongChanges(const Song& song) {
        delete lastSongFixed;
        lastSongFixed = new Song(song);
    }

    void MusicManager::updateLastSongAlbumArtWithImage(const string& img) {
        if (!lastSongFixed) return;

        // code taken from moveImage(img)
        stringstream destpath;
        boost::filesystem::path src(img);
        destpath << albumSubPathForImage << "/" << cleanDirName(src.filename().string());

        Album* album = lastSongFixed->getAlbum();
        album->setCoverFilepath(destpath.str());
        album->update();
    }

    void MusicManager::clearLastSongs() {
      delete lastParsedSong;
      lastParsedSong = NULL;
    }

# pragma mark paths

std::string MusicManager::cleanDirName(const std::string& s) {
  std::string name(s);
  std::replace(name.begin(), name.end(), '/', '-');
  std::replace(name.begin(), name.end(), ':', ' ');
  std::replace(name.begin(), name.end(), '\\', ' ');
  boost::regex charsToRemoveRegex("[!@#$%^&*?'\";]");
  boost::regex prefixSpacesRegex("^[ ]*");
  boost::regex suffixSpacesRegex("[ ]*$");
  name = boost::regex_replace(name, charsToRemoveRegex, "");
  name = boost::regex_replace(name, prefixSpacesRegex, "");
  name = boost::regex_replace(name, suffixSpacesRegex, "");
  // remove nonascii characters
  name.erase(remove_if(name.begin(), name.end(), [](auto c) { return !isascii(c); }), name.end());
  return name;
}

string MusicManager::getCopyToPath() {
    boost::filesystem::path dir(SoulSifterSettings::getInstance().get<string>("music.dir"));
    if (boost::filesystem::exists(dir) && boost::filesystem::is_directory(dir)) {
        return SoulSifterSettings::getInstance().get<string>("music.dir");
    }
    boost::filesystem::path staging(SoulSifterSettings::getInstance().get<string>("staging.dir"));
    if (boost::filesystem::exists(staging) && boost::filesystem::is_directory(staging)) {
        return SoulSifterSettings::getInstance().get<string>("staging.dir");
    }
    return "";
}

bool MusicManager::updateAlbumCover(const string& img, Album* album) {
  // create path
  std::ostringstream ssdirpath;
  std::string albumartist = album->getArtist().length() > 0 ? album->getArtist() : "_compilations_";
  std::string albumname = album->getName();
  ssdirpath << album->getBasicGenre()->getName() << "/" << cleanDirName(albumartist) << "/" << cleanDirName(albumname);
  std::string albumSubPathForImage = ssdirpath.str();
  transform(albumSubPathForImage.begin(), albumSubPathForImage.end(), albumSubPathForImage.begin(), ::tolower);

  // move file to dest
  try {
    std::stringstream destpath;
    boost::filesystem::path src(img);
    albumSubPathForImage = albumSubPathForImage + "/" + cleanDirName(src.filename().string());
    destpath << SoulSifterSettings::getInstance().get<string>("music.dir") << albumSubPathForImage;
    boost::filesystem::path dest(destpath.str());
    boost::filesystem::rename(src, dest);

    album->setCoverFilepath(albumSubPathForImage);
    album->update();

    return true;
  } catch (const boost::filesystem::filesystem_error& ex) {
    LOG(WARNING) << ex.what() << '\n';
  }
  return false;
}

// TODO shouldn't need to lower case full path
// TODO removed use of stagingPath. Add back feature?
bool MusicManager::moveSong(Song* song) {
    try {
        string albumPath;
        {
            ostringstream ssdirpath;
            string albumartist = song->getAlbum()->getArtist().length() > 0 ? song->getAlbum()->getArtist() : "_compilations_";
            string albumname = song->getAlbum()->getName();
            ssdirpath << song->getAlbum()->getBasicGenre()->getName() << "/" << cleanDirName(albumartist) << "/" << cleanDirName(albumname);
            albumSubPathForImage = ssdirpath.str();
            transform(albumSubPathForImage.begin(), albumSubPathForImage.end(), albumSubPathForImage.begin(), ::tolower);
            if (song->getAlbumPart() && song->getAlbumPart()->getName().length()) {
                string part = song->getAlbumPart()->getName();
                ssdirpath << "/" << cleanDirName(part);
            }
            albumPath = ssdirpath.str();
        }
        // create directory
        transform(albumPath.begin(), albumPath.end(), albumPath.begin(), ::tolower);
        string fullAlbumPath = SoulSifterSettings::getInstance().get<string>("music.dir") + albumPath;
        boost::filesystem::path dir(fullAlbumPath);
        if (!boost::filesystem::exists(dir)) {
            if (!boost::filesystem::create_directories(dir)) {
                LOG(WARNING) << "Error occurred while trying to create directory " << fullAlbumPath;
                return false;
            }
        } else if (!boost::filesystem::is_directory(dir)) {
            LOG(WARNING) << "Cannot move file b/c destination is not a directory " << fullAlbumPath;
            return false;
        }

        // move file to dest
        boost::filesystem::path src(song->getFilepath());
        string songPath = albumPath + "/" + cleanDirName(src.filename().string());
        stringstream destPath;
        destPath << SoulSifterSettings::getInstance().get<string>("music.dir") << songPath;
        boost::filesystem::path dest(destPath.str());
        boost::filesystem::rename(src, dest);

        // update song path
        song->setFilepath(songPath);

        return true;  //TODO better testing
    } catch (const boost::filesystem::filesystem_error& ex) {
        LOG(WARNING) << ex.what() << '\n';
    }
    return false;
}

    // TODO removed use of stagingPath. Add back feature?
    bool MusicManager::moveImage(const string& img) {
        try {
            // move file to dest
            stringstream destpath;
            boost::filesystem::path src(img);
            destpath << SoulSifterSettings::getInstance().get<string>("music.dir") << albumSubPathForImage << "/" << cleanDirName(src.filename().string());
            boost::filesystem::path dest(destpath.str());
            boost::filesystem::rename(src, dest);

            return true;
        } catch (const boost::filesystem::filesystem_error& ex) {
            LOG(WARNING) << ex.what() << '\n';
        }
        return false;
    }


# pragma mark db updates

void MusicManager::updateDatabaseBasicGenres() {
    // TODO the artists to genres
    try {
        boost::filesystem::path path(getCopyToPath());
        if (!boost::filesystem::exists(path)) {
            LOG(WARNING) << "Cannot update db basic genres because path does not exist: " << getCopyToPath();
            return;
        } else if (!boost::filesystem::is_directory(path)) {
            LOG(WARNING) << "Cannot update db basic genres because path is not a directory: " << getCopyToPath();
            return;
        }

        boost::filesystem::directory_iterator end; // default ctor yields past the end
        for (boost::filesystem::directory_iterator it(path); it != end; ++it) {
            if (boost::filesystem::is_directory(it->status())) {
                string filename = it->path().filename().string();
                const BasicGenre *genre = BasicGenre::findByName(filename);
                if (!genre) {
                    BasicGenre *basicGenre = new BasicGenre();
                    basicGenre->setName(filename);
                    basicGenre->save();
                    genre = BasicGenre::findByName(filename);
                }
                for (boost::filesystem::directory_iterator jt(it->path()); jt != end; ++jt) {
                    string artist = jt->path().filename().string();
                    artistToGenre[artist] = genre;
                }
            }
        }
    } catch (const boost::filesystem::filesystem_error& ex) {
        LOG(WARNING) << ex.what() << '\n';
    }
}

const BasicGenre* MusicManager::findBasicGenreForArtist(const string& artist) {
    return artistToGenre[artist];
}

bool MusicManager::splitArtistAndTitle(const string& songString, Song* updatedSong) {
  if (updatedSong->getArtist().length() > 0 || updatedSong->getTitle().length() > 0) return false;
  boost::regex splitRegex("^(.+) - (.+)$");
  boost::smatch match;
  if (boost::regex_search(songString, match, splitRegex, boost::match_extra)) {
    updatedSong->setArtist(dogatech::soulsifter::trim_copy(match[1]));
    updatedSong->setTitle(dogatech::soulsifter::trim_copy(match[2]));
    return true;
  }
  return false;
}

 /*
# pragma mark paths accessors

- (NSArray *)basicGenres {
    NSLog(@"musicManager.basicGenreList");
    if (!basicGenres) {
        [self initializePathMembers];
    }
    return basicGenres;
}

@end*/
}
}
