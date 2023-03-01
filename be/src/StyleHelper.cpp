//
//  StyleHelper.cpp
//  soul-sifter
//
//  Created by Robby Neale on 1/22/13.
//
//

#include "Style.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <g3log/g3log.hpp>

#include "MysqlAccess.h"

namespace dogatech {
namespace soulsifter {
    
# pragma mark helpers
    
    namespace {
        
        struct lessThanKey {
            inline bool operator()(const Style* v1, const Style* v2) {
                return (*v1) < (*v2);
            }
        };
        
        struct reIdLessThanKey {
            inline bool operator()(const Style* v1, const Style* v2) {
                return v1->getREId() < v2->getREId();
            }
        };
        
    }
    
# pragma mark operator overrides
    
    bool Style::operator<(const Style& style) const {
        return name.compare(style.getName()) < 0;
    }
    
    bool Style::operator>(const Style& style) const {
        return name.compare(style.getName()) > 0;
    }
    
# pragma mark static methods
    
    void Style::findAllSortedByName(vector<Style*>** stylesPtr) {
        vector<Style*>* styles = Style::findAll()->toVector();
        sort(styles->begin(), styles->end(), lessThanKey());
        (*stylesPtr) = styles;
    }
    
    void Style::findAllSortedByREId(vector<Style*>** stylesPtr) {
        vector<Style*>* styles = Style::findAll()->toVector();
        sort(styles->begin(), styles->end(), reIdLessThanKey());
        (*stylesPtr) = styles;
    }

    ResultSetIterator<Style>* Style::findAllParents() {
        for (int i = 0; i < 3; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select *, group_concat(children.childId) as childIds, group_concat(parents.parentId) as parentIds from Styles left outer join StyleChildren children on Styles.id = children.parentId left outer join StyleChildren parents on Styles.id = parents.childId group by Styles.id having parentIds is null");
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<Style> *dtrs = new ResultSetIterator<Style>(rs);
                return dtrs;
            } catch (sql::SQLException &e) {
                LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
                LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
                bool reconnected = MysqlAccess::getInstance().reconnect();
                LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
            }
        }
        LOG(FATAL) << "Unable to complete model operation";
    }

}  // namespace soulsifter
}  // namespace dogatech