//
//  Song.h
//  soul-sifter
//
//  Created by Robby Neale
//  Generated by generate_model.rb
//

#ifndef __soul_sifter__Song__
#define __soul_sifter__Song__

#include <set>
#include <string>
#include <vector>

#include "ResultSetIterator.h"

namespace sql {
    class ResultSet;
}

using namespace std;

namespace dogatech {
namespace soulsifter {

    class RESong;
    class Album;
    class AlbumPart;
    class MusicVideo;
    class Style;
    class Song;

    class Song {
    public:
        Song();
        explicit Song(const Song& song);
        void operator=(const Song& song);
        ~Song();
        void clear();

        static Song* findById(int id);
        static Song* findByFilepath(const string& filepath);
        static Song* findByGoogleSongId(const string& googleSongId);
        static Song* findByYoutubeId(const string& youtubeId);
        static Song* findBySpotifyId(const string& spotifyId);
        static Song* findByRESongId(int reSongId);
        static ResultSetIterator<Song>* findAll();

        int update();
        int save();
        // sync with db & return true if object needs saving or updating
        bool sync();

        explicit Song(RESong* song);

        static RESong* createRESongFromSong(Song& song);

        const string reAlbum();
        const string getDateAddedString() const;
        void setDateAddedToNow();

        int getId() const;
        void setId(int id);
        const string& getArtist() const;
        void setArtist(const string& artist);
        const string& getTrack() const;
        void setTrack(const string& track);
        const string& getTitle() const;
        void setTitle(const string& title);
        const string& getRemixer() const;
        void setRemixer(const string& remixer);
        const string& getFeaturing() const;
        void setFeaturing(const string& featuring);
        const string& getFilepath() const;
        void setFilepath(const string& filepath);
        int getRating() const;
        void setRating(int rating);
        time_t getDateAdded() const;
        void setDateAdded(time_t dateAdded);
        const string& getBpm() const;
        void setBpm(const string& bpm);
        const string& getTonicKey() const;
        void setTonicKey(const string& tonicKey);
        int getEnergy() const;
        void setEnergy(int energy);
        const string& getComments() const;
        void setComments(const string& comments);
        bool getTrashed() const;
        void setTrashed(bool trashed);
        bool getLowQuality() const;
        void setLowQuality(bool lowQuality);
        const string& getGoogleSongId() const;
        void setGoogleSongId(const string& googleSongId);
        const string& getYoutubeId() const;
        void setYoutubeId(const string& youtubeId);
        const string& getSpotifyId() const;
        void setSpotifyId(const string& spotifyId);
        int getDurationInMs() const;
        void setDurationInMs(int durationInMs);
        const string& getCurator() const;
        void setCurator(const string& curator);
        int getRESongId() const;
        void setRESongId(int reSongId);
        RESong* getRESong();
        RESong* getRESongConst() const;
        void setRESong(const RESong& reSong);
        void setRESong(RESong* reSong);  // takes ownership
        int getAlbumId() const;
        void setAlbumId(int albumId);
        Album* getAlbum();
        Album* getAlbumConst() const;
        void setAlbum(const Album& album);
        void setAlbum(Album* album);  // takes ownership
        int getAlbumPartId() const;
        void setAlbumPartId(int albumPartId);
        AlbumPart* getAlbumPart();
        AlbumPart* getAlbumPartConst() const;
        void setAlbumPart(const AlbumPart& albumPart);
        void setAlbumPart(AlbumPart* albumPart);  // takes ownership
        int getMusicVideoId() const;
        void setMusicVideoId(int musicVideoId);
        MusicVideo* getMusicVideo();
        MusicVideo* getMusicVideoConst() const;
        void setMusicVideo(const MusicVideo& musicVideo);
        void setMusicVideo(MusicVideo* musicVideo);  // takes ownership
        const vector<int>& getStyleIds() const;
        void setStyleIds(const vector<int>& styleIds);
        const vector<Style*>& getStyles();
        void setStyles(const vector<Style*>& styles);
        bool getBpmLock() const;
        void setBpmLock(bool bpmLock);
        bool getTonicKeyLock() const;
        void setTonicKeyLock(bool tonicKeyLock);
        int getDupeId() const;
        void setDupeId(int dupeId);
        Song* getDupe();
        Song* getDupeConst() const;
        void setDupe(const Song& dupe);
        void setDupe(Song* dupe);  // takes ownership

        friend ResultSetIterator<Song>;
        friend class SearchUtil;

    private:
        int id;
        string artist;
        string track;
        string title;
        string remixer;
        string featuring;
        string filepath;
        int rating;
        time_t dateAdded;
        string bpm;
        string tonicKey;
        int energy;
        string comments;
        bool trashed;
        bool lowQuality;
        string googleSongId;
        string youtubeId;
        string spotifyId;
        int durationInMs;
        string curator;
        int reSongId;
        RESong* reSong;
        int albumId;
        Album* album;
        int albumPartId;
        AlbumPart* albumPart;
        int musicVideoId;
        MusicVideo* musicVideo;
        vector<int> styleIds;
        vector<Style*> styles;
        bool bpmLock;
        bool tonicKeyLock;
        int dupeId;
        Song* dupe;

        static void populateFields(const sql::ResultSet* rs, Song* song);
    };

}
}

#endif /* defined(__soul_sifter__Song__) */
