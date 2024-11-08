//
//  BasicGenre.cpp
//  soul-sifter
//
//  Created by Robby Neale
//  Generated by generate_model.rb
//

#include "BasicGenre.h"

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

using namespace std;

namespace dogatech {
namespace soulsifter {

# pragma mark initialization

    BasicGenre::BasicGenre() :
    id(0),
    name() {
    }

    BasicGenre::BasicGenre(const BasicGenre& basicGenre) :
    id(basicGenre.getId()),
    name(basicGenre.getName()) {
    }

    void BasicGenre::operator=(const BasicGenre& basicGenre) {
        id = basicGenre.getId();
        name = basicGenre.getName();
    }

    BasicGenre::~BasicGenre() {
    }

    void BasicGenre::clear() {
        id = 0;
        name.clear();
    }

# pragma mark static methods

    void BasicGenre::populateFields(const sql::ResultSet* rs, BasicGenre* basicGenre) {
        basicGenre->setId(rs->getInt("id"));
        basicGenre->setName(rs->getString("name"));
    }

    BasicGenre* BasicGenre::findById(int id) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select BasicGenres.* from BasicGenres where BasicGenres.id = ?");
                ps->setInt(1, id);
                sql::ResultSet *rs = ps->executeQuery();
                BasicGenre *basicGenre = NULL;
                if (rs->next()) {
                    basicGenre = new BasicGenre();
                    populateFields(rs, basicGenre);
                }
                rs->close();
                delete rs;

                return basicGenre;
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

    BasicGenre* BasicGenre::findByName(const string& name) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select BasicGenres.* from BasicGenres where ifnull(name,'') = ifnull(?,'')");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                sql::ResultSet *rs = ps->executeQuery();
                BasicGenre *basicGenre = NULL;
                if (rs->next()) {
                    basicGenre = new BasicGenre();
                    populateFields(rs, basicGenre);
                }
                rs->close();
                delete rs;

                return basicGenre;
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

    ResultSetIterator<BasicGenre>* BasicGenre::findAll() {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select BasicGenres.* from BasicGenres");
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<BasicGenre> *dtrs = new ResultSetIterator<BasicGenre>(rs);
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

    int BasicGenre::update() {
        for (int i = 0; i < 2; ++i) {
            try {

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("update BasicGenres set name=? where id=?");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                ps->setInt(2, id);
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

    int BasicGenre::save() {
        for (int i = 0; i < 2; ++i) {
            try {

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("insert into BasicGenres (name) values (?)");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                int saved = ps->executeUpdate();
                if (!saved) {
                    LOG(WARNING) << "Not able to save basicGenre";
                    return saved;
                } else {
                    id = MysqlAccess::getInstance().getLastInsertId();
                    if (id == 0) {
                        LOG(WARNING) << "Inserted basicGenre, but unable to retreive inserted ID.";
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

    bool BasicGenre::sync() {
        return true;
    }


# pragma mark accessors

    int BasicGenre::getId() const { return id; }
    void BasicGenre::setId(const int id) { this->id = id; }

    const string& BasicGenre::getName() const { return name; }
    void BasicGenre::setName(const string& name) { this->name = name; }

}
}
