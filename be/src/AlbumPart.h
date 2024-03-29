//
//  AlbumPart.h
//  soul-sifter
//
//  Created by Robby Neale
//  Generated by generate_model.rb
//

#ifndef __soul_sifter__AlbumPart__
#define __soul_sifter__AlbumPart__

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

    class Album;

    class AlbumPart {
    public:
        AlbumPart();
        explicit AlbumPart(const AlbumPart& albumPart);
        void operator=(const AlbumPart& albumPart);
        ~AlbumPart();
        void clear();

        static AlbumPart* findById(int id);
        static AlbumPart* findByPosAndAlbumId(const string& pos, int albumId);
        static ResultSetIterator<AlbumPart>* findByPos(string pos);
        static ResultSetIterator<AlbumPart>* findByAlbumId(int albumId);
        static ResultSetIterator<AlbumPart>* findAll();

        int update();
        int save();
        // sync with db & return true if object needs saving or updating
        bool sync();

        int getId() const;
        void setId(int id);
        const string& getPos() const;
        void setPos(const string& pos);
        const string& getName() const;
        void setName(const string& name);
        int getAlbumId() const;
        void setAlbumId(int albumId);
        Album* getAlbum();
        Album* getAlbumConst() const;
        void setAlbum(const Album& album);
        void setAlbum(Album* album);  // takes ownership

        friend ResultSetIterator<AlbumPart>;
        friend class SearchUtil;

    private:
        int id;
        string pos;
        string name;
        int albumId;
        Album* album;

        static void populateFields(const sql::ResultSet* rs, AlbumPart* albumPart);
    };

}
}

#endif /* defined(__soul_sifter__AlbumPart__) */
