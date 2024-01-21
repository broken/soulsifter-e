#ifndef __soul_sifter__MusicService__
#define __soul_sifter__MusicService__

#include <string>

using namespace std;

namespace dogatech {
namespace soulsifter {

class Album;

class MusicService {
public:
  static string cleanDirName(const string& name);
  static bool updateAlbumCover(const string& img, Album* album);
  // TODO static void replaceSongFile(Song* song, const std::string& filepath);
  static bool moveAlbum(Album* album, std::function<void(std::string)> errorCallback = {});
};

}  // namespace soulsifter
}  // namespace dogatech

#endif /* defined(__soul_sifter__MusicService__) */
