//
//  Search.h
//  soul-sifter
//
//  Created by Robby Neale on 7/7/14.
//
//

#ifndef __soul_sifter__Search__
#define __soul_sifter__Search__

#include <functional>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace dogatech {
namespace soulsifter {
class Song;
class Style;

enum OrderBy { DATE_ADDED, RELEASE_DATE, RANDOM };

class SearchUtil {
public:
  static vector<Song*>* searchSongs(const string& query,
                                    const int bpm,
                                    const string& key,
                                    const vector<Style*>& styles,
                                    const vector<Song*>& songsToOmit,
                                    int limit,
                                    int energy = 0,
                                    const bool musicVideoMode = false,
                                    int orderBy = DATE_ADDED,
                                    std::function<void(string)> errorCallback = {});
};
  
}  // namespace soulsifter
}  // namespace dogatech

#endif /* defined(__soul_sifter__Search__) */
