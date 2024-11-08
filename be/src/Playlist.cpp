//
//  Playlist.cpp
//  soul-sifter
//
//  Created by Robby Neale
//  Generated by generate_model.rb
//

#include "Playlist.h"

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
#include "Style.h"

using namespace std;

namespace dogatech {
namespace soulsifter {

# pragma mark initialization

    Playlist::Playlist() :
    id(0),
    name(),
    query(),
    gmusicId(),
    youtubeId(),
    spotifyId(),
    styleIds(),
    styles() {
    }

    Playlist::Playlist(const Playlist& playlist) :
    id(playlist.getId()),
    name(playlist.getName()),
    query(playlist.getQuery()),
    gmusicId(playlist.getGmusicId()),
    youtubeId(playlist.getYoutubeId()),
    spotifyId(playlist.getSpotifyId()),
    styleIds(playlist.getStyleIds()),
    styles() {
    }

    void Playlist::operator=(const Playlist& playlist) {
        id = playlist.getId();
        name = playlist.getName();
        query = playlist.getQuery();
        gmusicId = playlist.getGmusicId();
        youtubeId = playlist.getYoutubeId();
        spotifyId = playlist.getSpotifyId();
        styleIds = playlist.getStyleIds();
        deleteVectorPointers(&styles);
    }

    Playlist::~Playlist() {
        while (!styles.empty()) delete styles.back(), styles.pop_back();
    }

    void Playlist::clear() {
        id = 0;
        name.clear();
        query.clear();
        gmusicId.clear();
        youtubeId.clear();
        spotifyId.clear();
        styleIds.clear();
        deleteVectorPointers(&styles);
    }

# pragma mark static methods

    void Playlist::populateFields(const sql::ResultSet* rs, Playlist* playlist) {
        playlist->setId(rs->getInt("id"));
        playlist->setName(rs->getString("name"));
        playlist->setQuery(rs->getString("query"));
        playlist->setGmusicId(rs->getString("gmusicId"));
        playlist->setYoutubeId(rs->getString("youtubeId"));
        playlist->setSpotifyId(rs->getString("spotifyId"));
        if (!rs->isNull("styleIds")) {
            string csv = rs->getString("styleIds");
            istringstream iss(csv);
            string id;
            while (getline(iss, id, ',')) {
              playlist->styleIds.push_back(atoi(id.c_str()));
            }
        }
    }

    Playlist* Playlist::findById(int id) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Playlists.*, group_concat(distinct(styles.styleId)) as styleIds from Playlists left outer join PlaylistStyles styles on Playlists.id = styles.playlistId where Playlists.id = ? group by Playlists.id");
                ps->setInt(1, id);
                sql::ResultSet *rs = ps->executeQuery();
                Playlist *playlist = NULL;
                if (rs->next()) {
                    playlist = new Playlist();
                    populateFields(rs, playlist);
                }
                rs->close();
                delete rs;

                return playlist;
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

    Playlist* Playlist::findByName(const string& name) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Playlists.*, group_concat(distinct(styles.styleId)) as styleIds from Playlists left outer join PlaylistStyles styles on Playlists.id = styles.playlistId where Playlists.name = ? group by Playlists.id");
                ps->setString(1, name);
                sql::ResultSet *rs = ps->executeQuery();
                Playlist *playlist = NULL;
                if (rs->next()) {
                    playlist = new Playlist();
                    populateFields(rs, playlist);
                }
                rs->close();
                delete rs;

                return playlist;
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

    Playlist* Playlist::findBySpotifyId(const string& spotifyId) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Playlists.*, group_concat(distinct(styles.styleId)) as styleIds from Playlists left outer join PlaylistStyles styles on Playlists.id = styles.playlistId where Playlists.spotifyId = ? group by Playlists.id");
                ps->setString(1, spotifyId);
                sql::ResultSet *rs = ps->executeQuery();
                Playlist *playlist = NULL;
                if (rs->next()) {
                    playlist = new Playlist();
                    populateFields(rs, playlist);
                }
                rs->close();
                delete rs;

                return playlist;
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

    ResultSetIterator<Playlist>* Playlist::findAll() {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Playlists.*, group_concat(distinct(styles.styleId)) as styleIds from Playlists left outer join PlaylistStyles styles on Playlists.id = styles.playlistId group by Playlists.id");
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<Playlist> *dtrs = new ResultSetIterator<Playlist>(rs);
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

    int Playlist::update() {
        for (int i = 0; i < 2; ++i) {
            try {

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("update Playlists set name=?, query=?, gmusicId=?, youtubeId=?, spotifyId=? where id=?");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (!query.empty()) ps->setString(2, query);
                else ps->setNull(2, sql::DataType::VARCHAR);
                if (!gmusicId.empty()) ps->setString(3, gmusicId);
                else ps->setNull(3, sql::DataType::VARCHAR);
                if (!youtubeId.empty()) ps->setString(4, youtubeId);
                else ps->setNull(4, sql::DataType::VARCHAR);
                if (!spotifyId.empty()) ps->setString(5, spotifyId);
                else ps->setNull(5, sql::DataType::VARCHAR);
                ps->setInt(6, id);
                int result = ps->executeUpdate();
                if (!styleIds.empty()) {
                    stringstream ss("insert ignore into PlaylistStyles (playlistId, styleId) values (?, ?)", ios_base::app | ios_base::out | ios_base::ate);
                    for (size_t i = 1; i < styleIds.size(); ++i) {
                        ss << ", (?, ?)";
                    }
                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                    for (size_t i = 0; i < styleIds.size(); ++i) {
                        ps->setInt(i * 2 + 1, id);
                        ps->setInt(i * 2 + 2, styleIds[i]);
                    }
                    ps->executeUpdate();
                    ss.str(std::string());
                    ss << "delete ignore from PlaylistStyles where playlistId = ? and styleId not in (?";
                    for (size_t i = 1; i < styleIds.size(); ++i) {
                        ss << ", ?";
                    }
                    ss << ")";
                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                    ps->setInt(1, id);
                    for (size_t i = 0; i < styleIds.size(); ++i) {
                        ps->setInt(i + 2, styleIds[i]);
                    }
                    ps->executeUpdate();
                } else {
                    ps = MysqlAccess::getInstance().getPreparedStatement("delete ignore from PlaylistStyles where playlistId = ?");
                    ps->setInt(1, id);
                    ps->executeUpdate();
                }
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

    int Playlist::save() {
        for (int i = 0; i < 2; ++i) {
            try {

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("insert into Playlists (name, query, gmusicId, youtubeId, spotifyId) values (?, ?, ?, ?, ?)");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (!query.empty()) ps->setString(2, query);
                else ps->setNull(2, sql::DataType::VARCHAR);
                if (!gmusicId.empty()) ps->setString(3, gmusicId);
                else ps->setNull(3, sql::DataType::VARCHAR);
                if (!youtubeId.empty()) ps->setString(4, youtubeId);
                else ps->setNull(4, sql::DataType::VARCHAR);
                if (!spotifyId.empty()) ps->setString(5, spotifyId);
                else ps->setNull(5, sql::DataType::VARCHAR);
                int saved = ps->executeUpdate();
                if (!saved) {
                    LOG(WARNING) << "Not able to save playlist";
                    return saved;
                } else {
                    id = MysqlAccess::getInstance().getLastInsertId();
                    if (id == 0) {
                        LOG(WARNING) << "Inserted playlist, but unable to retreive inserted ID.";
                        return saved;
                    }
                    if (!styleIds.empty()) {
                        stringstream ss("insert ignore into PlaylistStyles (playlistId, styleId) values (?, ?)", ios_base::app | ios_base::out | ios_base::ate);
                        for (size_t i = 1; i < styleIds.size(); ++i) {
                            ss << ", (?, ?)";
                        }
                        ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                        for (size_t i = 0; i < styleIds.size(); ++i) {
                            ps->setInt(i * 2 + 1, id);
                            ps->setInt(i * 2 + 2, styleIds[i]);
                        }
                        if (!ps->executeUpdate()) {
                            LOG(WARNING) << "Did not save style for playlist " << id;
                        }
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

    bool Playlist::sync() {
        return true;
    }

    int Playlist::erase() {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("delete from Playlists where id=?");
                ps->setInt(1, id);
                int erased = ps->executeUpdate();
                if (!erased) {
                    LOG(WARNING) << "Not able to erase playlist";
                }
                return erased;
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


# pragma mark accessors

    int Playlist::getId() const { return id; }
    void Playlist::setId(const int id) { this->id = id; }

    const string& Playlist::getName() const { return name; }
    void Playlist::setName(const string& name) { this->name = name; }

    const string& Playlist::getQuery() const { return query; }
    void Playlist::setQuery(const string& query) { this->query = query; }

    const string& Playlist::getGmusicId() const { return gmusicId; }
    void Playlist::setGmusicId(const string& gmusicId) { this->gmusicId = gmusicId; }

    const string& Playlist::getYoutubeId() const { return youtubeId; }
    void Playlist::setYoutubeId(const string& youtubeId) { this->youtubeId = youtubeId; }

    const string& Playlist::getSpotifyId() const { return spotifyId; }
    void Playlist::setSpotifyId(const string& spotifyId) { this->spotifyId = spotifyId; }

    const vector<int>& Playlist::getStyleIds() const { return styleIds; }
    void Playlist::setStyleIds(const vector<int>& styleIds) {
        while (!styles.empty()) delete styles.back(), styles.pop_back();
        this->styleIds.clear();
        this->styleIds = styleIds;
    }

    const vector<Style*>& Playlist::getStyles() {
        if (styles.empty() && !styleIds.empty()) {
            for (vector<int>::const_iterator it = styleIds.begin(); it != styleIds.end(); ++it) {
                styles.push_back(Style::findById(*it));
            }
        }
        return styles;
    }
    void Playlist::setStyles(const vector<Style*>& styles) {
        deleteVectorPointers<Style*>(&this->styles);
        this->styles = styles;
        this->styleIds.clear();
        for (vector<Style*>::const_iterator it = styles.begin(); it != styles.end(); ++it) {
            this->styleIds.push_back((*it)->getId());
        }
    }

}
}
