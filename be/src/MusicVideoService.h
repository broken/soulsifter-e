#ifndef __soul_sifter__MusicVideoService__
#define __soul_sifter__MusicVideoService__

#include <future>
#include <string>
#include <vector>

#include "JobQueue.h"

using namespace std;

namespace dogatech {
namespace soulsifter {

class Song;
class MusicVideo;

class MusicVideoService {
public:
  static MusicVideo* associateYouTubeVideo(Song* song, const string& url);
  static vector<string> downloadAudio(const string& url);
  static std::future<std::vector<std::string>> downloadAudioAsync(const std::string& url);

private:
  static JobQueue<std::vector<std::string>> job_queue;
};

}  // namespace soulsifter
}  // namespace dogatech

#endif /* defined(__soul_sifter__MusicVideoService__) */
