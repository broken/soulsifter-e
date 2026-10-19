//
//  SongHelper.cpp
//  soul-sifter
//
//  Created by Robby Neale on 1/22/13.
//
//

#include "Song.h"

#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>

#include "MysqlAccess.h"

namespace dogatech {
namespace soulsifter {

    const string Song::getDateAddedString() const { return stringFromTime(dateAdded); }
    void Song::setDateAddedToNow() { dateAdded = time(0); }

    bool Song::isTemporary() const {
        return isTemporaryFilepath(filepath);
    }

    bool Song::isTemporaryFilepath(const string& filepath) {
        if (filepath.empty()) return false;
        // In SoulSifter, permanent collection songs are stored as paths relative
        // to dir.music. Any absolute path (starting with / or a Windows drive letter)
        // indicates a temporary or externally imported song.
        if (filepath[0] == '/') return true;
        if (filepath.length() >= 2 && filepath[1] == ':') return true;
        return false;
    }

}  // namespace soulsifter
}  // namespace dogatech
