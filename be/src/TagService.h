#ifndef soul_sifter_TagService_h
#define soul_sifter_TagService_h

#include <functional>

namespace dogatech {
namespace soulsifter {
    
class Song;

class TagService {
public:
  static void readId3v2Tag(Song* song);
  static void writeId3v2Tag(Song* song);
  static void updateSongAttributesFromTags(std::function<void(float)> progressCallback = {});
};
    
}  // namespace soulsifter
}  // namespace dogatech

#endif
