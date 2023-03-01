//
//  SongHelper.cpp
//  soul-sifter
//
//  Created by Robby Neale on 1/22/13.
//
//

#include "Song.h"

#include <sstream>

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "Album.h"
#include "AlbumPart.h"
#include "BasicGenre.h"
#include "MusicVideo.h"
#include "MysqlAccess.h"
#include "RESong.h"
#include "Style.h"

namespace dogatech {
namespace soulsifter {
    
    Song::Song(RESong* song) :
    id(0),
    artist(song->getArtist()),
    track(song->getTrack()),
    title(song->getTitle()),
    remixer(song->getRemix()),
    featuring(song->getFeaturing()),
    filepath(song->getFilename()),
    reSongId(song->getId()),
    reSong(new RESong(*song)),
    rating(song->getRating()),
    dateAdded(timeFromString(song->getDateAdded())),
    bpm(song->getBpmStart()),
    energy(0),
    comments(song->getComments()),
    trashed(!song->getDisabled().compare("yes")),
    lowQuality(false),
    googleSongId(),
    youtubeId(),
    spotifyId(),
    durationInMs(0),
    curator(),
    albumId(0),
    album(NULL),
    albumPartId(0),
    albumPart(NULL),
    musicVideoId(0),
    musicVideo(NULL),
    styles(),
    bpmLock(false),
    tonicKeyLock(false) {
        
        // styles
        vector<Style*>* allStyles;
        Style::findAllSortedByREId(&allStyles);
        unsigned long pos = song->getStylesBitmask().find('1', 0);
        while (pos != string::npos) {
            // TODO add back addStyleById(allStyles->at(pos)->getId());
            pos = song->getStylesBitmask().find('1', ++pos);
        }
        while (!allStyles->empty()) delete allStyles->back(), allStyles->pop_back();
        
        // album
        album = Album::findByNameAndArtist(song->getAlbum(), song->getArtist());
        if (!album) album = new Album();
        album->setName(song->getAlbum());
        album->setLabel(song->getLabel());
        album->setCatalogId(song->getCatalogId());
        albumId = album->getId();
        // release dates
        string releaseDate = song->getReleaseDate();
        if (releaseDate.length() >=4) {
            album->setReleaseDateYear(atoi(releaseDate.substr(0,4).c_str()));
            if (releaseDate.length() >= 6) {
                album->setReleaseDateMonth(atoi(releaseDate.substr(4,2).c_str()));
                if (releaseDate.length() >= 8) {
                    album->setReleaseDateDay(atoi(releaseDate.substr(6,2).c_str()));
                }
            }
        }
      
        // basic genre
        const BasicGenre *genre = BasicGenre::findByFilepath(song->getFilename());
        if (genre)
            album->setBasicGenre(*genre);
      
        // date added
        dateAdded = timeFromString(song->getDateAdded());
      
        // keys
        // TODO tonicKey(<RE KEY TO KEY>);
    }
    
    const string Song::reAlbum() {
        if (!getAlbum()->getName().empty()) {
            return getAlbum()->getName();
        } else {
            return getTitle();
        }
    }
    
    const string Song::getDateAddedString() const { return stringFromTime(dateAdded); }
    void Song::setDateAddedToNow() { dateAdded = time(0); }
    
    RESong* Song::createRESongFromSong(Song& song) {
        RESong* re = new RESong();
        re->setId(0);
        re->setArtist(song.getArtist());
        re->setAlbum(song.reAlbum());
        re->setTrack(song.getTrack());
        re->setTitle(song.getTitle());
        re->setFilename(song.getFilepath());
        re->setRating(song.getRating());
        re->setDateAdded(song.getDateAddedString());
        re->setCatalogId(song.getAlbum()->getCatalogId());
        re->setLabel(song.getAlbum()->getLabel());
        re->setRemix(song.getRemixer());
        re->setComments(song.getComments());
        re->setBpmStart(song.getBpm());
        re->setKeyStart(song.getTonicKey());
        re->setReleaseDate(song.getAlbum()->reReleaseDate());
        re->setFeaturing(song.getFeaturing());
        re->setDisabled(song.getTrashed() ? "yes" : "no");
        return re;
    }

}  // namespace soulsifter
}  // namespace dogatech
