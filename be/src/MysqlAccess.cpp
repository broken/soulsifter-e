//
//  MysqlAccess.cpp
//  soul-sifter
//
//  Created by Robby Neale on 11/11/12.
//
//

#include "MysqlAccess.h"

#include <map>
#include <string>

#include <boost/date_time.hpp>
#include <boost/regex.hpp>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/metadata.h>
#include <jdbc/cppconn/resultset_metadata.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/warning.h>
#include <g3log/g3log.hpp>
#include <jdbc/mysql_driver.h>

#include "SoulSifterSettings.h"

namespace dogatech {
namespace soulsifter {

    namespace {
        const boost::regex TZ_REGEX(" \\w{3}$");
    }

# pragma mark public helpers

    time_t timeFromString(const std::string& str) {
        // so strptime can't get timezones right with dst, and boost fails when tz is defined
        if (boost::regex_search(str, TZ_REGEX)) {
            struct tm dt;
            memset(&dt, 0, sizeof(dt));
            strptime(str.c_str(), "%Y-%m-%d %X", &dt);
            return mktime(&dt);
        } else {
            boost::posix_time::ptime ptimedate = boost::posix_time::time_from_string(str);
            struct tm dt = boost::posix_time::to_tm(ptimedate);
            return mktime(&dt);
        }
    }

    std::string stringFromTime(const time_t time) {
        struct tm dt = *localtime(&time);
        char buffer[80];
        memset(buffer, 0, 80);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &dt);
        std::string str(buffer);
        return str;
    }

# pragma mark initialization

MysqlAccess::MysqlAccess() :
driver(0),
connection(0),
preparedStatements() {
    connect();
}

MysqlAccess::~MysqlAccess() {
    disconnect();
    for (const std::pair<std::string, sql::PreparedStatement*>& entry : preparedStatements) {
        delete entry.second;
    }
}

# pragma mark connecting

bool MysqlAccess::connect() {
    // initiate url, user, password and database variables
    const std::string url(SoulSifterSettings::getInstance().get<std::string>("db.url"));
    const std::string user(SoulSifterSettings::getInstance().get<std::string>("db.user"));
    const std::string password(SoulSifterSettings::getInstance().get<std::string>("db.password"));
    const std::string database(SoulSifterSettings::getInstance().get<std::string>("db.name"));

    try {
        driver = get_driver_instance();
        driver->threadInit();

        // create a database connection using the Driver
        connection = driver->connect(url, user, password);
        connection->setAutoCommit(1);
        connection->setSchema(database);
    } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        return false;
	}
    return true;
}

bool MysqlAccess::disconnect() {
    try {
        connection->close();
        delete connection;  // this deletes the driver as well
        connection = nullptr;
        driver = nullptr;
    } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        return false;
    }
    return true;
}

bool MysqlAccess::reconnect() {
    try {
        LOG(INFO) << "Reconnecing a" << (connection->isValid() ? " valid" : "n invalid") << " connection.";
        const std::string database(SoulSifterSettings::getInstance().get<std::string>("db.name"));
        connection->reconnect();
        connection->setAutoCommit(1);
        connection->setSchema(database);
        preparedStatements.clear();
    } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        return false;
    }
    return true;
}

sql::PreparedStatement* MysqlAccess::getPreparedStatement(std::string query) {
    try {
        sql::PreparedStatement *ps;
        std::map<std::string, sql::PreparedStatement*>::iterator it = preparedStatements.find(query);
        if (it == preparedStatements.end()) {
            ps = connection->prepareStatement(query);
            preparedStatements.insert(std::pair<std::string, sql::PreparedStatement*>(query, ps));
        } else {
            ps = it->second;
        }
        return ps;
    } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        throw e;
    }
}

int MysqlAccess::getLastInsertId() {
    try {
        sql::PreparedStatement *ps = getPreparedStatement("select LAST_INSERT_ID()");
        sql::ResultSet *rs = ps->executeQuery();
        int id = 0;
        if (rs->next()) {
            id = rs->getInt(1);
        }
        rs->close();
        delete rs;
        return id;
    } catch (sql::SQLException &e) {
        LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
        LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
        throw e;
    }
}

}  // dogatech
}  // soulsifter
