#ifndef __soul_sifter__TempDbManager__
#define __soul_sifter__TempDbManager__

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dogatech {
namespace soulsifter {

struct TempDbSession {
  std::string dbName;
  std::string dirPath;
  std::string styleName;
  int styleId;
  int idOffset;
};

/**
 * Manages the temporary database sessions used for importing and analyzing
 * music collections from a user's filesystem.
 * 
 * How it works:
 * 1. When a user imports a directory, TempDbManager creates a new physical
 *    temporary MySQL database (e.g., ss_tmp_123456789).
 * 2. It clones all song, album, and style table schemas from the main database
 *    into this new temp database.
 * 3. It populates the temp tables with song metadata read from the audio files
 *    in the user's directory.
 * 4. It creates a unified view database (ss_unified) that dynamically
 *    combines data from the main database and all active temp databases.
 * 5. The active MySQL connection is switched to use this unified view, making
 *    temp files appear as part of the main collection.
 * 
 * Lifecycle:
 * - Import: Creates temp DB, populates with files, rebuilds unified views
 * - Eject: Drops the temp DB and removes its views from unified view
 * - Clean: Removes any orphaned temp DBs left by crashes
 */
class TempDbManager {
public:
  static TempDbManager& getInstance() {
    static TempDbManager instance;  // guaranteed to be destroyed
    // instantiated on first use
    return instance;
  }
  ~TempDbManager();

  // Imports a directory into a new temp database, creates views in a unified db,
  // and switches active DB connection.
  bool importDirectory(const std::string& dirPath, std::function<void(float)> progressCb = {});

  // Eject a specific temp database or all
  bool ejectTempDb(const std::string& dbName);
  bool ejectAll();

  // Clean any orphaned temp DBs from past sessions or crashes
  void cleanOrphanedTempDatabases();

  const std::vector<TempDbSession>& getActiveSessions() const { return sessions; }
  bool hasActiveTempDbs() const { return !sessions.empty(); }
  void updateDatabaseSchema();

private:
  TempDbManager();
  TempDbManager(const TempDbManager&) = delete;
  TempDbManager& operator=(const TempDbManager&) = delete;

  void createPhysicalTempDb(TempDbSession* session);
  bool dropDatabase(const std::string& dbName);
  bool rebuildUnifiedViews();
  int scanAndIngestFiles(const TempDbSession& session, std::function<void(float)> progressCb);

  std::vector<TempDbSession> sessions;
  std::string unifiedDbName;
  int nextIdOffsetMultiplier;
};

}  // namespace soulsifter
}  // namespace dogatech

#endif /* defined(__soul_sifter__TempDbManager__) */
