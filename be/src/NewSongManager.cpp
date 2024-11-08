#include "NewSongManager.h"

#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <g3log/g3log.hpp>

#include "Album.h"
#include "AlbumPart.h"
#include "AudioAnalyzer.h"
#include "BasicGenre.h"
#include "FilesToAdd.h"
#include "MusicManager.h"
#include "Song.h"
#include "SoulSifterSettings.h"

using namespace boost;
using namespace std;

namespace dogatech {
namespace soulsifter {

NewSongManager::NewSongManager()
    : hasMovedFile(false),
      filesToAdd(NULL) {
}

NewSongManager::~NewSongManager() {
  delete filesToAdd;
}

void NewSongManager::import(const vector<string>& filepaths) {
  delete filesToAdd;
  filesToAdd = new FilesToAdd();
  hasMovedFile = false;
  vector<boost::filesystem::path> paths;
  transform(filepaths.begin(), filepaths.end(), back_inserter(paths), [](const string& fp) { return boost::filesystem::path(fp); });
  preprocessAllFiles(paths);
  //loadCover();
  //[fileUrls removeAllObjects];
  // loadNextSong();
}

void NewSongManager::preprocessAllFiles(const vector<boost::filesystem::path>& filepaths) {

  for (const boost::filesystem::path& filepath : filepaths) {

    // delete files starting with period
    if (filepath.filename().string().at(0) == '.') {
      filesToTrash.push_back(filepath);
      continue;
    }

    // if file is directory, enumerate over it and add files
    if (boost::filesystem::is_directory(filepath)) {
      vector<boost::filesystem::path> dirContents;
      copy(boost::filesystem::directory_iterator(filepath), boost::filesystem::directory_iterator(), back_inserter(dirContents));
      preprocessAllFiles(dirContents);
      filesToTrash.push_back(filepath);
      continue;
    }

    // TODO unarchive zips & rars, adding extracted files
    /*if ([[NSPredicate predicateWithFormat:@"self matches[c] \".+\\.part\\d+\\.rar$\""] evaluateWithObject:[filepath lastPathComponent]]) {
      if ([[filepath path] hasSuffix:@".part1.rar"]) {
        [self preprocessAllFiles:[NSArray arrayWithObject:[ArchiveUtil unrarFile:filepath]]];
      }
      [filesToTrash addObject:filepath];
      continue;
    } else if ([[filepath pathExtension] isEqualToString:@"rar"]) {
      [self preprocessAllFiles:[NSArray arrayWithObject:[ArchiveUtil unrarFile:filepath]]];
      [filesToTrash addObject:filepath];
      continue;
    } else if ([[filepath pathExtension] isEqualToString:@"zip"]) {
      [self preprocessAllFiles:[NSArray arrayWithObject:[ArchiveUtil unzipFile:filepath]]];
      [filesToTrash addObject:filepath];
      continue;
    }*/

    // let FilesToAdd do the rest
    filesToAdd->addFile(filepath.string());
  }
}

bool NewSongManager::nextSong(Song* updatedSong, Song* originalSong) {
  if (filesToAdd == NULL) {
    return false;
  }
  // Try to pull the next songs first if there is one.
  string* path = NULL;
  if (filesToAdd->pullSong(&path)) {
    originalSong->clear();
    originalSong->setFilepath(*path);
    MusicManager::getInstance().readTagsFromSong(originalSong);
    MusicManager::getInstance().updateSongWithChanges(*originalSong, updatedSong);

    // Do not sync the song here. Since we just updated the album with possible
    // values, we may sync to an improper album, at which point things will be
    // fucked when we save.

    // TODO thread this as well?
    if (!originalSong->getDurationInMs()) {
      int duration = AudioAnalyzer::analyzeDuration(updatedSong);
      originalSong->setDurationInMs(duration);
    }

    return true;
  }

  if (filesToAdd->pullFile(&path)) {
    // TODO only move image if a song has been moved or a song has not been skipped
    if (hasMovedFile) {
      // TODO move to last parent (no subalbums)
      // straight move image to last directory
      MusicManager::getInstance().moveImage(*path);
      // TODO should this really happen here?
      // update album with cover art
      if (filesToAdd->coverPath() && !strcasecmp(path->c_str(), filesToAdd->coverPath()->c_str())) {
        MusicManager::getInstance().updateLastSongAlbumArtWithImage(*path);
      }
    }
    delete path;
    return nextSong(updatedSong, originalSong);  // should never return true
  }

  for (size_t i = 0; i < filesToTrash.size(); ++i) {
    if (!boost::filesystem::remove(filesToTrash[i])) {
      LOG(WARNING) << "Unable to delete file at " << filesToTrash[i];
    }
  }
  filesToTrash.clear();
  MusicManager::getInstance().clearLastSongs();
  return false;
}

// basic genre & album name must exist for moving the file. blank album artist moves file into _compilations_ directory.
bool NewSongManager::processSong(Song* song) {
  // move file
  MusicManager::getInstance().moveSong(song);
  hasMovedFile = true;

  // update tag
  MusicManager::getInstance().writeTagsToSong(song);

  // save song
  if (!song->getId()) {
    song->setDateAddedToNow();
    song->save();
    MusicManager::getInstance().setNewSongChanges(*song);
  } else {
    song->update();
  }

  if (song->getTrashed()) trashMusicFile(song);

  return true;
}

void NewSongManager::trashMusicFile(Song* song) {
  // create new file
  ofstream outputFile;
  string oldPath = SoulSifterSettings::getInstance().get<string>("dir.music") + song->getFilepath();
  string newPath = oldPath + ".txt";
  outputFile.open(newPath);
  outputFile.close();

  // trash old file
  if (remove(oldPath.c_str()) != 0) {
    LOG(WARNING) << "Unable to delete trashed song file at " << oldPath;
  }

  song->setTrashed(true);
  song->setYoutubeId("");
  song->setFilepath(song->getFilepath() + ".txt");
  song->update();
}

string NewSongManager::coverImagePath() const {
  return *filesToAdd->coverPath();
}

}  // namespace soulsifter
}  // namespace dogatech
