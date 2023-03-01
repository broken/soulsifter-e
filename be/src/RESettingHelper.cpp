//
//  RESettingHelper.cpp
//  soul-sifter
//
//  Created by Robby Neale on 1/22/13.
//
//

#include "RESetting.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "MysqlAccess.h"


namespace dogatech {
namespace soulsifter {

# pragma mark static methods

    void RESetting::findAll(vector<const RESetting*>* settings) {
        sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select * from RESettings");
        sql::ResultSet *rs = ps->executeQuery();
        while (rs->next()) {
            RESetting *setting = new RESetting();
            populateFields(rs, setting);
            settings->push_back(setting);
        }
        rs->close();
        delete rs;
    }

# pragma mark accessors

    string& RESetting::getValueRef() { return value; }
}
}