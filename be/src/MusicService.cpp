#include "MusicService.h"

#include <stdlib.h>
#include <string>

#include "Album.h"
#include "MusicManager.h"

namespace dogatech {
namespace soulsifter {
namespace {

std::string gen_random(const int len) {
  static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);
  for (int i = 0; i < len; ++i) {
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }
  return tmp_s;
}

}  // anon namespace

std::string MusicService::cleanDirName(const std::string& name) {
  std::string s(name);
  for (size_t i = 0; i < s.size(); i++) {
    if ((s[i] < 'A' || s[i] > 'Z') &&
        (s[i] < 'a' || s[i] > 'z') &&
        (s[i] < '0' || s[i] > '9') &&
        s[i] != '.' && s[i] != '-' && s[i] != ' ' && s[i] != '_' &&
        s[i] != '(' && s[i] != ')' && s[i] != '[' && s[i] != ']' &&
        s[i] != ',' && s[i] != '+' && s[i] != '&') {
      s.erase(i, 1);
      i--;
    }
  }
  if (s.size() < 5) {
    s = gen_random(5) + s;
  }
  return s;
}

bool MusicService::updateAlbumCover(const string& img, Album* album, std::function<void(std::string)> errorCallback) {
  return MusicManager::updateAlbumCover(img, album);
}

bool MusicService::moveAlbum(Album* album, std::function<void(std::string)> errorCallback) {
  return MusicManager::moveAlbum(album, errorCallback);
}

}  // namespace soulsifter
}  // namespace dogatech
