//
//  RESongHelper.cpp
//  soul-sifter
//
//  Created by Robby Neale on 1/22/13.
//
//

#include "RESong.h"

#include <sstream>

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <g3log/g3log.hpp>

#include "MysqlAccess.h"
#include "Style.h"

namespace dogatech {
namespace soulsifter {
    
    const int RESong::maxREId() {
        sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select max(id) from RESongs");
        sql::ResultSet *rs = ps->executeQuery();
        int max = 0;
        if (rs->next())
            max = rs->getInt(1);
        rs->close();
        delete rs;
        return max;
    }
    
    void RESong::setStylesBitmaskFromDb() {
        sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select count(*) from Styles");
        sql::ResultSet *rs = ps->executeQuery();
        int stylesCount = 0;
        if (rs->next()) {
            stylesCount = rs->getInt(1);
        } else {
            LOG(WARNING) << "Unable to get total count of styles";
            return;
        }
        rs->close();
        delete rs;
        
        // this next statement always returns false even though it sets the variable correctly
        MysqlAccess::getInstance().getPreparedStatement("set @n=0")->execute();
        // the order of the styles in this query needs to be the order they're written out to the RE
        // db to generate the correct styles bitmask
        ps = MysqlAccess::getInstance().getPreparedStatement("select os.n from songs s join songstyles ss on s.id=ss.songid join (select @n:=1+@n n, id from styles order by reid) os on ss.styleid=os.id where s.resongid=? order by os.n");
        ps->setInt(1, id);
        rs = ps->executeQuery();
        int nextStyleBit = -1;
        if (rs->next())
            nextStyleBit = rs->getInt(1);
        stringstream ss;
        for (int i = 0; i < stylesCount; ++i) {
            if (i == nextStyleBit - 1) {
                ss << "1";
                if (rs->next())
                    nextStyleBit = rs->getInt(1);
            } else {
                ss << "0";
            }
        }
        rs->close();
        delete rs;
        
        setStylesBitmask(ss.str());
    }
}
}