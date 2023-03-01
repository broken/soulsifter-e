//
//  AlbumHelper.cpp
//  soul-sifter
//
//  Created by Robby Neale on 1/21/13.
//
//

#include "Album.h"

#include <sstream>

#include "BasicGenre.h"

namespace dogatech {
namespace soulsifter {
    
    const string Album::reReleaseDate() const {
        ostringstream oss;
        if (releaseDateYear) {
            oss << releaseDateYear;
            if (releaseDateMonth) {
                if (releaseDateMonth < 10) oss << "0";
                oss << releaseDateMonth;
                if (releaseDateDay) {
                    if (releaseDateDay < 10) oss << "0";
                    oss << releaseDateDay;
                }
            }
        }
        return oss.str();
    }
    
}
}