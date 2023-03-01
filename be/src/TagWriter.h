//
//  TagWriter.h
//  soul-sifter
//
//  Created by Robby Neale on 2/15/13.
//
//

#ifndef __soul_sifter__TagWriter__
#define __soul_sifter__TagWriter__

#include "MusicManager.h"

namespace dogatech {
namespace soulsifter {
    
    class TagWriter {
    public:
        TagWriter();
        
        bool writeAll();
        
        unsigned long getSrcOffset() {
            return offset;
        }
        bool isProcessing();
        unsigned long getSrcLength() {
            return length;
        }
        
    private:
        bool processing;
        unsigned long length;
        unsigned long offset;
        
        void computeSrcLength();
    };
    
}
}

#endif /* defined(__soul_sifter__TagWriter__) */
