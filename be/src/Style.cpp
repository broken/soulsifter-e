//
//  Style.cpp
//  soul-sifter
//
//  Created by Robby Neale
//  Generated by generate_model.rb
//

#include "Style.h"

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

    Style::Style() :
    id(0),
    name(),
    reId(0),
    reLabel(),
    description(),
    childIds(),
    children(),
    parentIds(),
    parents() {
    }

    Style::Style(const Style& style) :
    id(style.getId()),
    name(style.getName()),
    reId(style.getREId()),
    reLabel(style.getRELabel()),
    description(style.getDescription()),
    childIds(style.getChildIds()),
    children(),
    parentIds(style.getParentIds()),
    parents() {
    }

    void Style::operator=(const Style& style) {
        id = style.getId();
        name = style.getName();
        reId = style.getREId();
        reLabel = style.getRELabel();
        description = style.getDescription();
        childIds = style.getChildIds();
        deleteVectorPointers(&children);
        parentIds = style.getParentIds();
        deleteVectorPointers(&parents);
    }

    Style::~Style() {
        while (!children.empty()) delete children.back(), children.pop_back();
        while (!parents.empty()) delete parents.back(), parents.pop_back();
    }

    void Style::clear() {
        id = 0;
        name.clear();
        reId = 0;
        reLabel.clear();
        description.clear();
        childIds.clear();
        deleteVectorPointers(&children);
        parentIds.clear();
        deleteVectorPointers(&parents);
    }

# pragma mark static methods

    void Style::populateFields(const sql::ResultSet* rs, Style* style) {
        style->setId(rs->getInt("id"));
        style->setName(rs->getString("name"));
        style->setREId(rs->getInt("reId"));
        style->setRELabel(rs->getString("reLabel"));
        style->setDescription(rs->getString("description"));
        if (!rs->isNull("childIds")) {
            string csv = rs->getString("childIds");
            istringstream iss(csv);
            string id;
            while (getline(iss, id, ',')) {
              style->childIds.push_back(atoi(id.c_str()));
            }
        }
        if (!rs->isNull("parentIds")) {
            string csv = rs->getString("parentIds");
            istringstream iss(csv);
            string id;
            while (getline(iss, id, ',')) {
              style->parentIds.push_back(atoi(id.c_str()));
            }
        }
    }

    Style* Style::findById(int id) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Styles.*, group_concat(distinct(children.childId)) as childIds, group_concat(distinct(parents.parentId)) as parentIds from Styles left outer join StyleChildren children on Styles.id = children.parentId left outer join StyleChildren parents on Styles.id = parents.childId where Styles.id = ? group by Styles.id");
                ps->setInt(1, id);
                sql::ResultSet *rs = ps->executeQuery();
                Style *style = NULL;
                if (rs->next()) {
                    style = new Style();
                    populateFields(rs, style);
                }
                rs->close();
                delete rs;

                return style;
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

    Style* Style::findByREId(int reId) {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Styles.*, group_concat(distinct(children.childId)) as childIds, group_concat(distinct(parents.parentId)) as parentIds from Styles left outer join StyleChildren children on Styles.id = children.parentId left outer join StyleChildren parents on Styles.id = parents.childId where ifnull(reId,0) = ifnull(?,0) group by Styles.id");
                if (reId > 0) ps->setInt(1, reId);
                else ps->setNull(1, sql::DataType::INTEGER);
                sql::ResultSet *rs = ps->executeQuery();
                Style *style = NULL;
                if (rs->next()) {
                    style = new Style();
                    populateFields(rs, style);
                }
                rs->close();
                delete rs;

                return style;
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

    ResultSetIterator<Style>* Style::findAll() {
        for (int i = 0; i < 2; ++i) {
            try {
                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("select Styles.*, group_concat(distinct(children.childId)) as childIds, group_concat(distinct(parents.parentId)) as parentIds from Styles left outer join StyleChildren children on Styles.id = children.parentId left outer join StyleChildren parents on Styles.id = parents.childId group by Styles.id");
                sql::ResultSet *rs = ps->executeQuery();
                ResultSetIterator<Style> *dtrs = new ResultSetIterator<Style>(rs);
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

    int Style::update() {
        for (int i = 0; i < 2; ++i) {
            try {

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("update Styles set name=?, reId=?, reLabel=?, description=? where id=?");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (reId > 0) ps->setInt(2, reId);
                else ps->setNull(2, sql::DataType::INTEGER);
                if (!reLabel.empty()) ps->setString(3, reLabel);
                else ps->setNull(3, sql::DataType::VARCHAR);
                if (!description.empty()) ps->setString(4, description);
                else ps->setNull(4, sql::DataType::VARCHAR);
                ps->setInt(5, id);
                int result = ps->executeUpdate();
                if (!childIds.empty()) {
                    stringstream ss("insert ignore into StyleChildren (parentId, childId) values (?, ?)", ios_base::app | ios_base::out | ios_base::ate);
                    for (size_t i = 1; i < childIds.size(); ++i) {
                        ss << ", (?, ?)";
                    }
                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                    for (size_t i = 0; i < childIds.size(); ++i) {
                        ps->setInt(i * 2 + 1, id);
                        ps->setInt(i * 2 + 2, childIds[i]);
                    }
                    ps->executeUpdate();
                    ss.str(std::string());
                    ss << "delete ignore from StyleChildren where parentId = ? and childId not in (?";
                    for (size_t i = 1; i < childIds.size(); ++i) {
                        ss << ", ?";
                    }
                    ss << ")";
                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                    ps->setInt(1, id);
                    for (size_t i = 0; i < childIds.size(); ++i) {
                        ps->setInt(i + 2, childIds[i]);
                    }
                    ps->executeUpdate();
                } else {
                    ps = MysqlAccess::getInstance().getPreparedStatement("delete ignore from StyleChildren where parentId = ?");
                    ps->setInt(1, id);
                    ps->executeUpdate();
                }
                if (!parentIds.empty()) {
                    stringstream ss("insert ignore into StyleChildren (childId, parentId) values (?, ?)", ios_base::app | ios_base::out | ios_base::ate);
                    for (size_t i = 1; i < parentIds.size(); ++i) {
                        ss << ", (?, ?)";
                    }
                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                    for (size_t i = 0; i < parentIds.size(); ++i) {
                        ps->setInt(i * 2 + 1, id);
                        ps->setInt(i * 2 + 2, parentIds[i]);
                    }
                    ps->executeUpdate();
                    ss.str(std::string());
                    ss << "delete ignore from StyleChildren where childId = ? and parentId not in (?";
                    for (size_t i = 1; i < parentIds.size(); ++i) {
                        ss << ", ?";
                    }
                    ss << ")";
                    ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                    ps->setInt(1, id);
                    for (size_t i = 0; i < parentIds.size(); ++i) {
                        ps->setInt(i + 2, parentIds[i]);
                    }
                    ps->executeUpdate();
                } else {
                    ps = MysqlAccess::getInstance().getPreparedStatement("delete ignore from StyleChildren where childId = ?");
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

    int Style::save() {
        for (int i = 0; i < 2; ++i) {
            try {

                sql::PreparedStatement *ps = MysqlAccess::getInstance().getPreparedStatement("insert into Styles (name, reId, reLabel, description) values (?, ?, ?, ?)");
                if (!name.empty()) ps->setString(1, name);
                else ps->setNull(1, sql::DataType::VARCHAR);
                if (reId > 0) ps->setInt(2, reId);
                else ps->setNull(2, sql::DataType::INTEGER);
                if (!reLabel.empty()) ps->setString(3, reLabel);
                else ps->setNull(3, sql::DataType::VARCHAR);
                if (!description.empty()) ps->setString(4, description);
                else ps->setNull(4, sql::DataType::VARCHAR);
                int saved = ps->executeUpdate();
                if (!saved) {
                    LOG(WARNING) << "Not able to save style";
                    return saved;
                } else {
                    id = MysqlAccess::getInstance().getLastInsertId();
                    if (id == 0) {
                        LOG(WARNING) << "Inserted style, but unable to retreive inserted ID.";
                        return saved;
                    }
                    if (!childIds.empty()) {
                        stringstream ss("insert ignore into StyleChildren (parentId, childId) values (?, ?)", ios_base::app | ios_base::out | ios_base::ate);
                        for (size_t i = 1; i < childIds.size(); ++i) {
                            ss << ", (?, ?)";
                        }
                        ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                        for (size_t i = 0; i < childIds.size(); ++i) {
                            ps->setInt(i * 2 + 1, id);
                            ps->setInt(i * 2 + 2, childIds[i]);
                        }
                        if (!ps->executeUpdate()) {
                            LOG(WARNING) << "Did not save child for style " << id;
                        }
                    }
                    if (!parentIds.empty()) {
                        stringstream ss("insert ignore into StyleChildren (childId, parentId) values (?, ?)", ios_base::app | ios_base::out | ios_base::ate);
                        for (size_t i = 1; i < parentIds.size(); ++i) {
                            ss << ", (?, ?)";
                        }
                        ps = MysqlAccess::getInstance().getPreparedStatement(ss.str());
                        for (size_t i = 0; i < parentIds.size(); ++i) {
                            ps->setInt(i * 2 + 1, id);
                            ps->setInt(i * 2 + 2, parentIds[i]);
                        }
                        if (!ps->executeUpdate()) {
                            LOG(WARNING) << "Did not save parent for style " << id;
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

    bool Style::sync() {
        Style* style = findById(id);
        if (!style) style = findByREId(getREId());
        if (!style) return true;

        // check fields
        bool needsUpdate = false;
        boost::regex decimal("(-?\\d+)\\.?\\d*");
        boost::smatch match1;
        boost::smatch match2;
        if (id != style->getId()) {
            id = style->getId();
        }
        if (name.compare(style->getName())  && (!boost::regex_match(name, match1, decimal) || !boost::regex_match(style->getName(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            LOG(INFO) << "updating style " << id << " name from " << style->getName() << " to " << name;
            needsUpdate = true;
        }
        if (reId != style->getREId()) {
            LOG(INFO) << "updating style " << id << " reId from " << style->getREId() << " to " << reId;
            needsUpdate = true;
        }
        if (reLabel.compare(style->getRELabel())  && (!boost::regex_match(reLabel, match1, decimal) || !boost::regex_match(style->getRELabel(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            LOG(INFO) << "updating style " << id << " reLabel from " << style->getRELabel() << " to " << reLabel;
            needsUpdate = true;
        }
        if (description.compare(style->getDescription())  && (!boost::regex_match(description, match1, decimal) || !boost::regex_match(style->getDescription(), match2, decimal) || match1[1].str().compare(match2[1].str()))) {
            LOG(INFO) << "updating style " << id << " description from " << style->getDescription() << " to " << description;
            needsUpdate = true;
        }
        if (!equivalentVectors<int>(childIds, style->getChildIds())) {
            LOG(INFO) << "updating style " << id << " childIds";
            needsUpdate = true;
        }
        if (!equivalentVectors<int>(parentIds, style->getParentIds())) {
            LOG(INFO) << "updating style " << id << " parentIds";
            needsUpdate = true;
        }
        return needsUpdate;
    }


# pragma mark accessors

    int Style::getId() const { return id; }
    void Style::setId(const int id) { this->id = id; }

    const string& Style::getName() const { return name; }
    void Style::setName(const string& name) { this->name = name; }

    int Style::getREId() const { return reId; }
    void Style::setREId(const int reId) { this->reId = reId; }

    const string& Style::getRELabel() const { return reLabel; }
    void Style::setRELabel(const string& reLabel) { this->reLabel = reLabel; }

    const string& Style::getDescription() const { return description; }
    void Style::setDescription(const string& description) { this->description = description; }

    const vector<int>& Style::getChildIds() const { return childIds; }
    void Style::setChildIds(const vector<int>& childIds) {
        while (!children.empty()) delete children.back(), children.pop_back();
        this->childIds.clear();
        this->childIds = childIds;
    }

    const vector<Style*>& Style::getChildren() {
        if (children.empty() && !childIds.empty()) {
            for (vector<int>::const_iterator it = childIds.begin(); it != childIds.end(); ++it) {
                children.push_back(Style::findById(*it));
            }
        }
        return children;
    }
    void Style::setChildren(const vector<Style*>& children) {
        deleteVectorPointers<Style*>(&this->children);
        this->children = children;
        this->childIds.clear();
        for (vector<Style*>::const_iterator it = children.begin(); it != children.end(); ++it) {
            this->childIds.push_back((*it)->getId());
        }
    }

    const vector<int>& Style::getParentIds() const { return parentIds; }
    void Style::setParentIds(const vector<int>& parentIds) {
        while (!parents.empty()) delete parents.back(), parents.pop_back();
        this->parentIds.clear();
        this->parentIds = parentIds;
    }

    const vector<Style*>& Style::getParents() {
        if (parents.empty() && !parentIds.empty()) {
            for (vector<int>::const_iterator it = parentIds.begin(); it != parentIds.end(); ++it) {
                parents.push_back(Style::findById(*it));
            }
        }
        return parents;
    }
    void Style::setParents(const vector<Style*>& parents) {
        deleteVectorPointers<Style*>(&this->parents);
        this->parents = parents;
        this->parentIds.clear();
        for (vector<Style*>::const_iterator it = parents.begin(); it != parents.end(); ++it) {
            this->parentIds.push_back((*it)->getId());
        }
    }

}
}
