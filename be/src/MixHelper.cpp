//
//  MixHelper.cpp
//  soul-sifter
//
//  Created by Robby Neale on 1/22/13.
//
//

#include "Mix.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <g3log/g3log.hpp>

#include "MysqlAccess.h"

#include "Song.h"

namespace dogatech {
namespace soulsifter {
    
    int Mix::mixoutCountForRESongId(int outRESongId) {
        for (int i = 0; i < 3; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select count(*) from Songs s join Mixes m on (s.id = m.outSongId) where s.reSongId = ?");
                ps->setInt(1, outRESongId);
                sql::ResultSet *rs = ps->executeQuery();
                int count = 0;
                if (rs->next()) {
                    count = rs->getInt(1);
                }
                rs->close();
                delete rs;
                
                return count;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
            }
        }
        LOG(FATAL) << "Unable to complete model operation";
    }
    
}
}