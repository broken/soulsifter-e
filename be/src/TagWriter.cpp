//
//  TagWriter.cpp
//  soul-sifter
//
//  Created by Robby Neale on 2/15/13.
//
//

#include "TagWriter.h"

#include <g3log/g3log.hpp>

#include "RESong.h"
#include "Song.h"

namespace dogatech {
namespace soulsifter {
    
# pragma mark initialization
    
    TagWriter::TagWriter() :
    processing(false),
    length(0),
    offset(0) {
    }
    
# pragma mark tag writer
    
    void TagWriter::computeSrcLength() {
        // TODO
        length = 15101L;
    }
    
    bool TagWriter::writeAll() {
        LOG(INFO) << "TagWriter::writeAll";
        processing = true;
        offset = 0;
        computeSrcLength();
        
        ResultSetIterator<RESong>* songs = RESong::findAll();
        RESong reSong;
        while (songs->next(&reSong)) {
            Song* song = Song::findByRESongId(reSong.getId());
            MusicManager::getInstance().writeTagsToSong(song);
            delete song;
            offset++;
        }
        delete songs;
        
        processing = false;
        length = 0;
        offset = 0;
        return true;
    }
    
    bool TagWriter::isProcessing() {
        return processing;
    }
    
}
}
