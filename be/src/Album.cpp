//
//  Album.cpp
//  soul-sifter
//
//  Created by Robby Neale
//  Generated by generate_model.rb
//

#include "Album.h"

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
#include "BasicGenre.h"

using namespace std;

namespace dogatech {
namespace soulsifter {

# pragma mark initialization

    Album::Album() :
    id(0),
    name(),
    artist(),
    coverFilepath(),
    mixed(false),
    label(),
    catalogId(),
    releaseDateYear(0),
    releaseDateMonth(0),
    releaseDateDay(0),
    basicGenreId(0),
    basicGenre(NULL) {
    }

    Album::Album(const Album& album) :
    id(album.getId()),
    name(album.getName()),
    artist(album.getArtist()),
    coverFilepath(album.getCoverFilepath()),
    mixed(album.getMixed()),
    label(album.getLabel()),
    catalogId(album.getCatalogId()),
    releaseDateYear(album.getReleaseDateYear()),
    releaseDateMonth(album.getReleaseDateMonth()),
    releaseDateDay(album.getReleaseDateDay()),
    basicGenreId(album.getBasicGenreId()),
    basicGenre(NULL) {
        if (album.basicGenre) setBasicGenre(*album.basicGenre);
    }

    void Album::operator=(const Album& album) {
        id = album.getId();
        name = album.getName();
        artist = album.getArtist();
        coverFilepath = album.getCoverFilepath();
        mixed = album.getMixed();
        label = album.getLabel();
        catalogId = album.getCatalogId();
        releaseDateYear = album.getReleaseDateYear();
        releaseDateMonth = album.getReleaseDateMonth();
        releaseDateDay = album.getReleaseDateDay();
        basicGenreId = album.getBasicGenreId();
        if (!album.getBasicGenreId() && album.basicGenre) {
            if (!basicGenre) basicGenre = new BasicGenre(*album.basicGenre);
            else *basicGenre = *album.basicGenre;
        } else {
            delete basicGenre;
            basicGenre = NULL;
        }
    }

    Album::~Album() {
        delete basicGenre;
        basicGenre = NULL;
    }

    void Album::clear() {
        id = 0;
        name.clear();
        artist.clear();
        coverFilepath.clear();
        mixed = false;
        label.clear();
        catalogId.clear();
        releaseDateYear = 0;
        releaseDateMonth = 0;
        releaseDateDay = 0;
        basicGenreId = 0;
        delete basicGenre;
        basicGenre = NULL;
    }

# pragma mark static methods

    void Album::populateFields(const sql::ResultSet* rs, Album* album) {
        album->setId(rs->getInt("id"));
        album->setName(rs->getString("name"));
        album->setArtist(rs->getString("artist"));
        album->setCoverFilepath(rs->getString("coverFilepath"));
        album->setMixed(rs->getBoolean("mixed"));
        album->setLabel(rs->getString("label"));
        album->setCatalogId(rs->getString("catalogId"));
        album->setReleaseDateYear(rs->getInt("releaseDateYear"));
        album->setReleaseDateMonth(rs->getInt("releaseDateMonth"));
        album->setReleaseDateDay(rs->getInt("releaseDateDay"));
        album->setBasicGenreId(rs->getInt("basicGenreId"));
    }

    Album* Album::findById(int id) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Albums.* from Albums where Albums.id = ?");
                ps->setInt(1, id);
                sql::ResultSet *rs = ps->executeQuery();
                Album *album = NULL;
                if (rs->next()) {
                    album = new Album();
                    populateFields(rs, album);
                }
                rs->close();
                delete rs;

                return album;
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

    Album* Album::findByCoverFilepath(const string& coverFilepath) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Albums.* from Albums where Albums.coverFilepath = ?");
                ps->setString(1, coverFilepath);
                sql::ResultSet *rs = ps->executeQuery();
                Album *album = NULL;
                if (rs->next()) {
                    album = new Album();
                    populateFields(rs, album);
                }
                rs->close();
                delete rs;

                return album;
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

    Album* Album::findByNameAndArtist(const string& name, const string& artist) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Albums.* from Albums where ifnull(name,'') = ifnull(?,'') and ifnull(artist,'') = ifnull(?,'')");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (!artist.empty()) ps->setString(2, artist);
                else ps->setNull(2, sql::DataType::VARCHAR);
                sql::ResultSet *rs = ps->executeQuery();
                Album *album = NULL;
                if (rs->next()) {
                    album = new Album();
                    populateFields(rs, album);
                }
                rs->close();
                delete rs;

                return album;
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

    ResultSetIterator<Album>* Album::findByName(string name) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Albums.* from Albums where ifnull(name,'') = ifnull(?,'')");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<Album> *dtrs = new ResultSetIterator<Album>(rs);
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

    ResultSetIterator<Album>* Album::findByArtist(string artist) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Albums.* from Albums where ifnull(artist,'') = ifnull(?,'')");
                if (!artist.empty()) ps->setString(1, artist);
                else ps->setNull(1, sql::DataType::VARCHAR);
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<Album> *dtrs = new ResultSetIterator<Album>(rs);
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

    ResultSetIterator<Album>* Album::findAll() {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Albums.* from Albums");
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<Album> *dtrs = new ResultSetIterator<Album>(rs);
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

    int Album::update() {
        for (int i = 0; i < 2; ++i) {
            try {
                if (basicGenre && basicGenre->sync()) {
                    if (basicGenre->getId()) {
                        basicGenre->update();
                    } else {
                        basicGenre->save();
                    }
                    basicGenreId = basicGenre->getId();
                } else if (!basicGenreId && basicGenre) {
                    basicGenreId = basicGenre->getId();
                }

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("update Albums set name=?, artist=?, coverFilepath=?, mixed=?, label=?, catalogId=?, releaseDateYear=?, releaseDateMonth=?, releaseDateDay=?, basicGenreId=? where id=?");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (!artist.empty()) ps->setString(2, artist);
                else ps->setNull(2, sql::DataType::VARCHAR);
                if (!coverFilepath.empty()) ps->setString(3, coverFilepath);
                else ps->setNull(3, sql::DataType::VARCHAR);
                ps->setBoolean(4, mixed);
                if (!label.empty()) ps->setString(5, label);
                else ps->setNull(5, sql::DataType::VARCHAR);
                if (!catalogId.empty()) ps->setString(6, catalogId);
                else ps->setNull(6, sql::DataType::VARCHAR);
                if (releaseDateYear > 0) ps->setInt(7, releaseDateYear);
                else ps->setNull(7, sql::DataType::INTEGER);
                if (releaseDateMonth > 0) ps->setInt(8, releaseDateMonth);
                else ps->setNull(8, sql::DataType::INTEGER);
                if (releaseDateDay > 0) ps->setInt(9, releaseDateDay);
                else ps->setNull(9, sql::DataType::INTEGER);
                if (basicGenreId > 0) ps->setInt(10, basicGenreId);
                else ps->setNull(10, sql::DataType::INTEGER);
                ps->setInt(11, id);
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

    int Album::save() {
        for (int i = 0; i < 2; ++i) {
            try {
                if (basicGenre && basicGenre->sync()) {
                    if (basicGenre->getId()) {
                        basicGenre->update();
                    } else {
                        basicGenre->save();
                    }
                    basicGenreId = basicGenre->getId();
                } else if (!basicGenreId && basicGenre) {
                    basicGenreId = basicGenre->getId();
                }

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("insert into Albums (name, artist, coverFilepath, mixed, label, catalogId, releaseDateYear, releaseDateMonth, releaseDateDay, basicGenreId) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (!artist.empty()) ps->setString(2, artist);
                else ps->setNull(2, sql::DataType::VARCHAR);
                if (!coverFilepath.empty()) ps->setString(3, coverFilepath);
                else ps->setNull(3, sql::DataType::VARCHAR);
                ps->setBoolean(4, mixed);
                if (!label.empty()) ps->setString(5, label);
                else ps->setNull(5, sql::DataType::VARCHAR);
                if (!catalogId.empty()) ps->setString(6, catalogId);
                else ps->setNull(6, sql::DataType::VARCHAR);
                if (releaseDateYear > 0) ps->setInt(7, releaseDateYear);
                else ps->setNull(7, sql::DataType::INTEGER);
                if (releaseDateMonth > 0) ps->setInt(8, releaseDateMonth);
                else ps->setNull(8, sql::DataType::INTEGER);
                if (releaseDateDay > 0) ps->setInt(9, releaseDateDay);
                else ps->setNull(9, sql::DataType::INTEGER);
                if (basicGenreId > 0) ps->setInt(10, basicGenreId);
                else ps->setNull(10, sql::DataType::INTEGER);
                int saved = ps->executeUpdate();
                if (!saved) {
                    LOG(WARNING) << "Not able to save album";
                    return saved;
                } else {
                    id = MysqlAccess::getInstance().getLastInsertId();
                    if (id == 0) {
                        LOG(WARNING) << "Inserted album, but unable to retreive inserted ID.";
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

    bool Album::sync() {
        Album* album = findById(id);
        if (!album) {
            if (!basicGenreId && basicGenre) {
                basicGenre->sync();
                basicGenreId = basicGenre->getId();
            }
        }
        if (!album) album = findByNameAndArtist(getName(), getArtist());
        if (!album) return true;

        // check fields
        bool needsUpdate = false;
        boost::regex decimal("(-?\\d+)\\.?\\d*");
        boost::smatch match1;
        boost::smatch match2;
        if (id != album->getId()) {
            if (id) {
                LOG(INFO) << "updating album " << id << " id from " << album->getId() << " to " << id;
                needsUpdate = true;
            } else {
                id = album->getId();
            }
        }
        if (name.compare(album->getName())  && (!boost::regex_match(name, match1, decimal) || !boost::regex_match(album->getName(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            if (!name.empty()) {
                LOG(INFO) << "updating album " << id << " name from " << album->getName() << " to " << name;
                needsUpdate = true;
            } else {
                name = album->getName();
            }
        }
        if (artist.compare(album->getArtist())  && (!boost::regex_match(artist, match1, decimal) || !boost::regex_match(album->getArtist(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            if (!artist.empty()) {
                LOG(INFO) << "updating album " << id << " artist from " << album->getArtist() << " to " << artist;
                needsUpdate = true;
            } else {
                artist = album->getArtist();
            }
        }
        if (coverFilepath.compare(album->getCoverFilepath())  && (!boost::regex_match(coverFilepath, match1, decimal) || !boost::regex_match(album->getCoverFilepath(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            if (!coverFilepath.empty()) {
                LOG(INFO) << "updating album " << id << " coverFilepath from " << album->getCoverFilepath() << " to " << coverFilepath;
                needsUpdate = true;
            } else {
                coverFilepath = album->getCoverFilepath();
            }
        }
        if (mixed != album->getMixed()) {
            if (mixed) {
                LOG(INFO) << "updating album " << id << " mixed from " << album->getMixed() << " to " << mixed;
                needsUpdate = true;
            } else {
                mixed = album->getMixed();
            }
        }
        if (label.compare(album->getLabel())  && (!boost::regex_match(label, match1, decimal) || !boost::regex_match(album->getLabel(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            if (!label.empty()) {
                LOG(INFO) << "updating album " << id << " label from " << album->getLabel() << " to " << label;
                needsUpdate = true;
            } else {
                label = album->getLabel();
            }
        }
        if (catalogId.compare(album->getCatalogId())  && (!boost::regex_match(catalogId, match1, decimal) || !boost::regex_match(album->getCatalogId(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            if (!catalogId.empty()) {
                LOG(INFO) << "updating album " << id << " catalogId from " << album->getCatalogId() << " to " << catalogId;
                needsUpdate = true;
            } else {
                catalogId = album->getCatalogId();
            }
        }
        if (releaseDateYear != album->getReleaseDateYear()) {
            if (releaseDateYear) {
                LOG(INFO) << "updating album " << id << " releaseDateYear from " << album->getReleaseDateYear() << " to " << releaseDateYear;
                needsUpdate = true;
            } else {
                releaseDateYear = album->getReleaseDateYear();
            }
        }
        if (releaseDateMonth != album->getReleaseDateMonth()) {
            if (releaseDateMonth) {
                LOG(INFO) << "updating album " << id << " releaseDateMonth from " << album->getReleaseDateMonth() << " to " << releaseDateMonth;
                needsUpdate = true;
            } else {
                releaseDateMonth = album->getReleaseDateMonth();
            }
        }
        if (releaseDateDay != album->getReleaseDateDay()) {
            if (releaseDateDay) {
                LOG(INFO) << "updating album " << id << " releaseDateDay from " << album->getReleaseDateDay() << " to " << releaseDateDay;
                needsUpdate = true;
            } else {
                releaseDateDay = album->getReleaseDateDay();
            }
        }
        if (basicGenreId != album->getBasicGenreId()) {
            if (basicGenreId) {
                LOG(INFO) << "updating album " << id << " basicGenreId from " << album->getBasicGenreId() << " to " << basicGenreId;
                needsUpdate = true;
            } else {
                basicGenreId = album->getBasicGenreId();
            }
        }
        if (basicGenre) needsUpdate |= basicGenre->sync();
        return needsUpdate;
    }


# pragma mark accessors

    int Album::getId() const { return id; }
    void Album::setId(const int id) { this->id = id; }

    const string& Album::getName() const { return name; }
    void Album::setName(const string& name) { this->name = name; }

    const string& Album::getArtist() const { return artist; }
    void Album::setArtist(const string& artist) { this->artist = artist; }

    const string& Album::getCoverFilepath() const { return coverFilepath; }
    void Album::setCoverFilepath(const string& coverFilepath) { this->coverFilepath = coverFilepath; }

    bool Album::getMixed() const { return mixed; }
    void Album::setMixed(const bool mixed) { this->mixed = mixed; }

    const string& Album::getLabel() const { return label; }
    void Album::setLabel(const string& label) { this->label = label; }

    const string& Album::getCatalogId() const { return catalogId; }
    void Album::setCatalogId(const string& catalogId) { this->catalogId = catalogId; }

    int Album::getReleaseDateYear() const { return releaseDateYear; }
    void Album::setReleaseDateYear(const int releaseDateYear) { this->releaseDateYear = releaseDateYear; }

    int Album::getReleaseDateMonth() const { return releaseDateMonth; }
    void Album::setReleaseDateMonth(const int releaseDateMonth) { this->releaseDateMonth = releaseDateMonth; }

    int Album::getReleaseDateDay() const { return releaseDateDay; }
    void Album::setReleaseDateDay(const int releaseDateDay) { this->releaseDateDay = releaseDateDay; }

    int Album::getBasicGenreId() const {
        return (!basicGenreId && basicGenre) ? basicGenre->getId() : basicGenreId;
    }
    void Album::setBasicGenreId(const int basicGenreId) {
        this->basicGenreId = basicGenreId;
        delete basicGenre;
        basicGenre = NULL;
    }

    BasicGenre* Album::getBasicGenre() {
        if (!basicGenre && basicGenreId) {
            basicGenre = BasicGenre::findById(basicGenreId);
        }
        return basicGenre;
    }
    BasicGenre* Album::getBasicGenreConst() const {
        return (!basicGenre && basicGenreId) ? BasicGenre::findById(basicGenreId) : basicGenre;
    }
    void Album::setBasicGenre(const BasicGenre& basicGenre) {
        this->basicGenreId = basicGenre.getId();
        delete this->basicGenre;
        this->basicGenre = new BasicGenre(basicGenre);
    }
    void Album::setBasicGenre(BasicGenre* basicGenre) {
        this->basicGenreId = basicGenre->getId();
        delete this->basicGenre;
        this->basicGenre = basicGenre;
    }

}
}
