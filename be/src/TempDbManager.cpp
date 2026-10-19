#include "TempDbManager.h"

#include <ctime>
#include <iostream>
#include <set>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <g3log/g3log.hpp>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

#include "Album.h"
#include "AlbumPart.h"
#include "MusicManager.h"
#include "MysqlAccess.h"
#include "Song.h"
#include "SoulSifterSettings.h"
#include "Style.h"
#include "TagService.h"

namespace fs = boost::filesystem;

namespace dogatech {
namespace soulsifter {

namespace {

bool isAudioFile(const fs::path& p) {
  std::string ext = p.extension().string();
  boost::algorithm::to_lower(ext);
  return (ext == ".mp3" || ext == ".m4a" || ext == ".flac" ||
          ext == ".aif" || ext == ".aiff" || ext == ".wav");
}

const std::vector<std::string> kPartitionedTables = {
  "Styles", "SongStyles", "Albums", "AlbumParts", "Songs"
};

}  // namespace

TempDbManager::TempDbManager() : nextIdOffsetMultiplier(1) {
  unifiedDbName = "ss_unified";
}

TempDbManager::~TempDbManager() {
  ejectAll();
}

bool TempDbManager::dropDatabase(const std::string& dbName) {
  try {
    std::unique_ptr<sql::Statement> stmt(MysqlAccess::getInstance().createStatement());
    stmt->execute("DROP DATABASE IF EXISTS `" + dbName + "`");
    LOG(INFO) << "Dropped database: " << dbName;
    return true;
  } catch (sql::SQLException& e) {
    LOG(WARNING) << "Failed to drop database " << dbName << ": " << e.what();
    return false;
  }
}

void TempDbManager::cleanOrphanedTempDatabases() {
  try {
    std::unique_ptr<sql::Statement> stmt(MysqlAccess::getInstance().createStatement());
    std::unique_ptr<sql::ResultSet> rs(stmt->executeQuery("SHOW DATABASES LIKE 'ss_tmp_%'"));
    while (rs->next()) {
      dropDatabase(rs->getString(1));
    }
    dropDatabase(unifiedDbName);
  } catch (sql::SQLException& e) {
    LOG(WARNING) << "Error while cleaning orphaned temp databases: " << e.what();
  }
}

bool TempDbManager::createPhysicalTempDb(TempDbSession* session) {
  try {
    std::unique_ptr<sql::Statement> stmt(MysqlAccess::getInstance().createStatement());
    stmt->execute("CREATE DATABASE `" + session->dbName + "`");

    // Clone tables from the core database
    for (const auto& tbl : kPartitionedTables) {
      stmt->execute("CREATE TABLE `" + session->dbName + "`.`" + tbl +
                    "` LIKE `" + SoulSifterSettings::getInstance().get<std::string>("db.name") + "`.`" + tbl + "`");
      stmt->execute("ALTER TABLE `" + session->dbName + "`.`" + tbl +
                    "` AUTO_INCREMENT = " + std::to_string(session->idOffset));
    }

    // Switch to temp database so Style/Song/Album saves go directly into it
    MysqlAccess::getInstance().switchDatabase(session->dbName);

    // Create the style "temp-<dir_name>"
    Style tempStyle;
    tempStyle.setName(session->styleName);
    tempStyle.setDescription("Temporary import for " + session->dirPath);
    int styleId = tempStyle.save();
    session->styleId = styleId;

    return true;
  } catch (sql::SQLException& e) {
    LOG(WARNING) << "Failed to create physical temp db " << session->dbName << ": " << e.what();
    return false;
  }
}

int TempDbManager::scanAndIngestFiles(const TempDbSession& session,
                                     std::function<void(float)> progressCb) {
  std::vector<fs::path> audioFiles;
  try {
    fs::path dir(session.dirPath);
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
      LOG(WARNING) << "Directory does not exist or is not a directory: " << session.dirPath;
      return 0;
    }

    for (fs::recursive_directory_iterator it(dir), end; it != end; ++it) {
      if (fs::is_regular_file(it->status()) && isAudioFile(it->path())) {
        audioFiles.push_back(it->path());
      }
    }
  } catch (const std::exception& e) {
    LOG(WARNING) << "Error traversing directory " << session.dirPath << ": " << e.what();
    return 0;
  }

  size_t total = audioFiles.size();
  LOG(INFO) << "Found " << total << " audio files in " << session.dirPath;

  int count = 0;
  for (size_t i = 0; i < total; ++i) {
    const fs::path& filePath = audioFiles[i];
    // update progress
    if (progressCb && (i % 8 == 0)) {
      progressCb(static_cast<float>(i) / total);
    }

    try {
      Song song;
      song.setFilepath(filePath.string());

      MusicManager::getInstance().readTagsFromSong(&song);

      // If albumPart has no info, clear it so it doesn't create empty records
      AlbumPart* albumPart = song.getAlbumPart();
      if (albumPart && albumPart->getPos().empty() && albumPart->getName().empty()) {
        song.setAlbumPart(nullptr);
      }

      // Add temp style ID
      std::vector<int> styles = song.getStyleIds();
      styles.push_back(session.styleId);
      song.setStyleIds(styles);

      song.save();
      count++;
    } catch (const std::exception& e) {
      LOG(WARNING) << "Error ingesting file " << filePath << ": " << e.what();
    }
  }

  LOG(INFO) << "Successfully ingested " << count << " songs into " << session.dbName;
  return count;
}

bool TempDbManager::rebuildUnifiedViews() {
  try {
    // If no temp sessions exist, drop unified database and switch back to core
    std::string coreDbName = SoulSifterSettings::getInstance().get<std::string>("db.name");
    if (sessions.empty()) {
      MysqlAccess::getInstance().switchDatabase(coreDbName);
      dropDatabase(unifiedDbName);
      return true;
    }

    // Ensure unified database exists
    std::unique_ptr<sql::Statement> stmt(MysqlAccess::getInstance().createStatement());
    stmt->execute("CREATE DATABASE IF NOT EXISTS `" + unifiedDbName + "`");

    // Tables that are partitioned across core and temp databases
    for (const auto& tbl : kPartitionedTables) {
      std::stringstream viewSql;
      viewSql << "CREATE OR REPLACE VIEW `" + unifiedDbName + "`.`" + tbl + "` AS ";
      viewSql << "SELECT * FROM `" + coreDbName + "`.`" + tbl + "` ";
      for (const auto& s : sessions) {
        viewSql << "UNION ALL SELECT * FROM `" + s.dbName + "`.`" + tbl + "` ";
      }
      stmt->execute(viewSql.str());
    }

    // Pass-through tables (all tables in coreDbName not in kPartitionedTables)
    std::unique_ptr<sql::ResultSet> rsTables(stmt->executeQuery(
        "SELECT TABLE_NAME FROM information_schema.TABLES WHERE TABLE_SCHEMA = '" + coreDbName + "' AND TABLE_TYPE = 'BASE TABLE'"));
    std::set<std::string> partitionedSet(kPartitionedTables.begin(), kPartitionedTables.end());
    while (rsTables->next()) {
      std::string tbl = rsTables->getString(1);
      if (partitionedSet.find(tbl) == partitionedSet.end()) {
        std::string viewSql = "CREATE OR REPLACE VIEW `" + unifiedDbName + "`.`" + tbl +
                              "` AS SELECT * FROM `" + coreDbName + "`.`" + tbl + "`";
        stmt->execute(viewSql);
      }
    }

    // Switch active connection to unified DB
    MysqlAccess::getInstance().switchDatabase(unifiedDbName);
    LOG(INFO) << "Successfully rebuilt unified views in: " << unifiedDbName;
    return true;
  } catch (sql::SQLException& e) {
    LOG(WARNING) << "Error rebuilding unified views: " << e.what();
    return false;
  }
}

bool TempDbManager::importDirectory(const std::string& dirPath,
                                   std::function<void(float)> progressCb) {
  fs::path pathObj(dirPath);
  std::string dirName = pathObj.filename().string();
  if (dirName.empty()) dirName = "drive";

  TempDbSession session;
  session.dbName = "ss_tmp_" + std::to_string(std::time(nullptr));
  session.dirPath = dirPath;
  session.styleName = "@temp-" + dirName;
  session.idOffset = 10000000 * (nextIdOffsetMultiplier++);

  LOG(INFO) << "Starting temporary import for " << dirPath << " into " << session.dbName;

  if (!createPhysicalTempDb(&session)) {
    LOG(WARNING) << "Failed to create temp database for session";
    return false;
  }

  scanAndIngestFiles(session, progressCb);

  sessions.push_back(session);

  return rebuildUnifiedViews();
}

bool TempDbManager::ejectTempDb(const std::string& dbName) {
  auto it = std::find_if(sessions.begin(), sessions.end(),
                         [&dbName](const TempDbSession& s) { return s.dbName == dbName; });
  if (it == sessions.end()) {
    LOG(WARNING) << "Session " << dbName << " not found to eject.";
    return false;
  }

  dropDatabase(it->dbName);
  sessions.erase(it);

  return rebuildUnifiedViews();
}

bool TempDbManager::ejectAll() {
  for (const auto& s : sessions) {
    dropDatabase(s.dbName);
  }
  sessions.clear();
  return rebuildUnifiedViews();
}

}  // namespace soulsifter
}  // namespace dogatech
