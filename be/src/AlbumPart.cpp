//
//  AlbumPart.cpp
//  soul-sifter
//
//  Created by Robby Neale
//  Generated by generate_model.rb
//

#include "AlbumPart.h"

#include <cmath>
#include <string>
#include <sstream>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/warning.h>
#include <g3log/g3log.hpp>

#include "AlertsChannel.h"
#include "MysqlAccess.h"
#include "DTVectorUtil.h"
#include "Album.h"

using namespace std;

namespace dogatech {
namespace soulsifter {

# pragma mark initialization

    AlbumPart::AlbumPart() :
    id(0),
    pos(),
    name(),
    albumId(0),
    album(NULL) {
    }

    AlbumPart::AlbumPart(const AlbumPart& albumPart) :
    id(albumPart.getId()),
    pos(albumPart.getPos()),
    name(albumPart.getName()),
    albumId(albumPart.getAlbumId()),
    album(NULL) {
        if (albumPart.album) setAlbum(*albumPart.album);
    }

    void AlbumPart::operator=(const AlbumPart& albumPart) {
        id = albumPart.getId();
        pos = albumPart.getPos();
        name = albumPart.getName();
        albumId = albumPart.getAlbumId();
        if (!albumPart.getAlbumId() && albumPart.album) {
            if (!album) album = new Album(*albumPart.album);
            else *album = *albumPart.album;
        } else {
            delete album;
            album = NULL;
        }
    }

    AlbumPart::~AlbumPart() {
        delete album;
        album = NULL;
    }

    void AlbumPart::clear() {
        id = 0;
        pos.clear();
        name.clear();
        albumId = 0;
        delete album;
        album = NULL;
    }

# pragma mark static methods

    void AlbumPart::populateFields(const sql::ResultSet* rs, AlbumPart* albumPart) {
        albumPart->setId(rs->getInt("id"));
        albumPart->setPos(rs->getString("pos"));
        albumPart->setName(rs->getString("name"));
        albumPart->setAlbumId(rs->getInt("albumId"));
    }

    AlbumPart* AlbumPart::findById(int id) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select AlbumParts.* from AlbumParts where AlbumParts.id = ?");
                ps->setInt(1, id);
                sql::ResultSet *rs = ps->executeQuery();
                AlbumPart *albumPart = NULL;
                if (rs->next()) {
                    albumPart = new AlbumPart();
                    populateFields(rs, albumPart);
                }
                rs->close();
                delete rs;

                return albumPart;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
                if (i == 1) AlertsChannel::getInstance().send(std::string("DB Error (") + __FILE__ + "::" + __func__ + ") : " + e.what());
            }
        }
        return NULL;
    }

    AlbumPart* AlbumPart::findByPosAndAlbumId(const string& pos, int albumId) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select AlbumParts.* from AlbumParts where ifnull(pos,'') = ifnull(?,'') and ifnull(albumId,0) = ifnull(?,0)");
                if (!pos.empty()) ps->setString(1, pos);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (albumId > 0) ps->setInt(2, albumId);
                else ps->setNull(2, sql::DataType::INTEGER);
                sql::ResultSet *rs = ps->executeQuery();
                AlbumPart *albumPart = NULL;
                if (rs->next()) {
                    albumPart = new AlbumPart();
                    populateFields(rs, albumPart);
                }
                rs->close();
                delete rs;

                return albumPart;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
                if (i == 1) AlertsChannel::getInstance().send(std::string("DB Error (") + __FILE__ + "::" + __func__ + ") : " + e.what());
            }
        }
        return NULL;
    }

    ResultSetIterator<AlbumPart>* AlbumPart::findByPos(string pos) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select AlbumParts.* from AlbumParts where ifnull(pos,'') = ifnull(?,'')");
                if (!pos.empty()) ps->setString(1, pos);
                else ps->setNull(1, sql::DataType::VARCHAR);
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<AlbumPart> *dtrs = new ResultSetIterator<AlbumPart>(rs);
                return dtrs;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
                if (i == 1) AlertsChannel::getInstance().send(std::string("DB Error (") + __FILE__ + "::" + __func__ + ") : " + e.what());
            }
        }
        return NULL;
    }

    ResultSetIterator<AlbumPart>* AlbumPart::findByAlbumId(int albumId) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select AlbumParts.* from AlbumParts where ifnull(albumId,0) = ifnull(?,0)");
                if (albumId > 0) ps->setInt(1, albumId);
                else ps->setNull(1, sql::DataType::INTEGER);
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<AlbumPart> *dtrs = new ResultSetIterator<AlbumPart>(rs);
                return dtrs;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
                if (i == 1) AlertsChannel::getInstance().send(std::string("DB Error (") + __FILE__ + "::" + __func__ + ") : " + e.what());
            }
        }
        return NULL;
    }

    ResultSetIterator<AlbumPart>* AlbumPart::findAll() {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select AlbumParts.* from AlbumParts");
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<AlbumPart> *dtrs = new ResultSetIterator<AlbumPart>(rs);
                return dtrs;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
                if (i == 1) AlertsChannel::getInstance().send(std::string("DB Error (") + __FILE__ + "::" + __func__ + ") : " + e.what());
            }
        }
        return NULL;
    }

# pragma mark persistence

    int AlbumPart::update() {
        for (int i = 0; i < 2; ++i) {
            try {
                if (album && album->sync()) {
                    if (album->getId()) {
                        album->update();
                    } else {
                        album->save();
                    }
                    albumId = album->getId();
                } else if (!albumId && album) {
                    albumId = album->getId();
                }

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("update AlbumParts set pos=?, name=?, albumId=? where id=?");
                if (!pos.empty()) ps->setString(1, pos);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (!name.empty()) ps->setString(2, name);
                else ps->setNull(2, sql::DataType::VARCHAR);
                if (albumId > 0) ps->setInt(3, albumId);
                else ps->setNull(3, sql::DataType::INTEGER);
                ps->setInt(4, id);
                int result = ps->executeUpdate();
                return result;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
                if (i == 1) AlertsChannel::getInstance().send(std::string("DB Error (") + __FILE__ + "::" + __func__ + ") : " + e.what());
            }
        }
        return 0;
    }

    int AlbumPart::save() {
        for (int i = 0; i < 2; ++i) {
            try {
                if (album && album->sync()) {
                    if (album->getId()) {
                        album->update();
                    } else {
                        album->save();
                    }
                    albumId = album->getId();
                } else if (!albumId && album) {
                    albumId = album->getId();
                }

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("insert into AlbumParts (pos, name, albumId) values (?, ?, ?)");
                if (!pos.empty()) ps->setString(1, pos);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (!name.empty()) ps->setString(2, name);
                else ps->setNull(2, sql::DataType::VARCHAR);
                if (albumId > 0) ps->setInt(3, albumId);
                else ps->setNull(3, sql::DataType::INTEGER);
                int saved = ps->executeUpdate();
                if (!saved) {
                    LOG(WARNING) << "Not able to save albumPart";
                    return saved;
                } else {
                    id = MysqlAccess::getInstance().getLastInsertId();
                    if (id == 0) {
                        LOG(WARNING) << "Inserted albumPart, but unable to retreive inserted ID.";
                        return saved;
                    }
                    return saved;
                }
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
                if (i == 1) AlertsChannel::getInstance().send(std::string("DB Error (") + __FILE__ + "::" + __func__ + ") : " + e.what());
            }
        }
        return 0;
    }

    bool AlbumPart::sync() {
        AlbumPart* albumPart = findById(id);
        if (!albumPart) {
            if (!albumId && album) {
                album->sync();
                albumId = album->getId();
            }
        }
        if (!albumPart) albumPart = findByPosAndAlbumId(getPos(), getAlbumId());
        if (!albumPart) return true;

        // check fields
        bool needsUpdate = false;
        boost::regex decimal("(-?\\d+)\\.?\\d*");
        boost::smatch match1;
        boost::smatch match2;
        if (id != albumPart->getId()) {
            if (id) {
                LOG(INFO) << "updating albumPart " << id << " id from " << albumPart->getId() << " to " << id;
                needsUpdate = true;
            } else {
                id = albumPart->getId();
            }
        }
        if (pos.compare(albumPart->getPos())  && (!boost::regex_match(pos, match1, decimal) || !boost::regex_match(albumPart->getPos(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            if (!pos.empty()) {
                LOG(INFO) << "updating albumPart " << id << " pos from " << albumPart->getPos() << " to " << pos;
                needsUpdate = true;
            } else {
                pos = albumPart->getPos();
            }
        }
        if (name.compare(albumPart->getName())  && (!boost::regex_match(name, match1, decimal) || !boost::regex_match(albumPart->getName(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            if (!name.empty()) {
                LOG(INFO) << "updating albumPart " << id << " name from " << albumPart->getName() << " to " << name;
                needsUpdate = true;
            } else {
                name = albumPart->getName();
            }
        }
        if (albumId != albumPart->getAlbumId()) {
            if (albumId) {
                LOG(INFO) << "updating albumPart " << id << " albumId from " << albumPart->getAlbumId() << " to " << albumId;
                needsUpdate = true;
            } else {
                albumId = albumPart->getAlbumId();
            }
        }
        if (album) needsUpdate |= album->sync();
        return needsUpdate;
    }


# pragma mark accessors

    int AlbumPart::getId() const { return id; }
    void AlbumPart::setId(const int id) { this->id = id; }

    const string& AlbumPart::getPos() const { return pos; }
    void AlbumPart::setPos(const string& pos) { this->pos = pos; }

    const string& AlbumPart::getName() const { return name; }
    void AlbumPart::setName(const string& name) { this->name = name; }

    int AlbumPart::getAlbumId() const {
        return (!albumId && album) ? album->getId() : albumId;
    }
    void AlbumPart::setAlbumId(const int albumId) {
        this->albumId = albumId;
        delete album;
        album = NULL;
    }

    Album* AlbumPart::getAlbum() {
        if (!album && albumId) {
            album = Album::findById(albumId);
        }
        return album;
    }
    Album* AlbumPart::getAlbumConst() const {
        return (!album && albumId) ? Album::findById(albumId) : album;
    }
    void AlbumPart::setAlbum(const Album& album) {
        this->albumId = album.getId();
        delete this->album;
        this->album = new Album(album);
    }
    void AlbumPart::setAlbum(Album* album) {
        this->albumId = album->getId();
        delete this->album;
        this->album = album;
    }

}
}
