//
//  FilesToAdd.h
//  soul-sifter
//
//  Created by Robby Neale on 8/10/13.
//
//

#ifndef __soul_sifter__FilesToAdd__
#define __soul_sifter__FilesToAdd__

#include <string>
#include <queue>

#include "Song.h"

using namespace std;

namespace dogatech {
  namespace soulsifter {
  
    class FilesToAdd {
    public:
      FilesToAdd();
      ~FilesToAdd();
      
      void clear();
    
      void addFile(const string& path);
      
      bool pullSong(string** song);
      bool pullFile(string** path);
      
      const string* coverPath() const;
      void switchCover();
    
    private:
      queue<string*> songs;
      vector<string*> images;
      queue<string*> misc;
      int coverIndex;
      // we use this to save the cover path after the file has been pulled
      string* cover;
    };
  
  }  // namespace soulsifter
}  // namespace dogatech

#endif /* defined(__soul_sifter__FilesToAdd__) */
