#include "SoulSifterSettings.h"

#include <cstdlib>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <g3log/g3log.hpp>

using namespace boost;
using namespace std;

namespace dogatech {
namespace soulsifter {

  SoulSifterSettings::SoulSifterSettings() {
    // first set defaults
    ptree.put("app.debug", false);
    //ptree.put("dir.music", "");
    //ptree.put("dir.mv", "");
    //ptree.put("dir.staging", "");
    //ptree.put("dir.stems", "");
    //ptree.put("dir.waveforms", "");
    ptree.put("dir.tmp", "/tmp");
    ptree.put("db.url", "localhost");
    ptree.put("db.user", "ss");
    ptree.put("db.name", "music");
    ptree.put("db.password", "pw");
    ptree.put("edit.autoAdd", false);
    ptree.put("edit.autoPlay", false);
    //ptree.put("feedly.userId", "");
    //ptree.put("feedly.devToken", "");
    //ptree.put("google.clientId", "");
    //ptree.put("google.clientSecret", "");
    //ptree.put("google.oauthAccessToken", "");
    //ptree.put("google.oauthAccessTokenExpiration", "0");
    //ptree.put("google.oauthRefreshToken", "");
    //ptree.put("google.appKey", "");
    //ptree.put("google.email", "");
    //ptree.put("google.androidId", "");
    ptree.put("search.energyGap", 1);
    ptree.put("search.includeUnknownKeys", true);
    ptree.put("songList.column.bpm", false);
    ptree.put("songList.limit", 128);
    ptree.put("songList.searchOnSelect", false);
    ptree.put("songList.showComments", false);
    //ptree.put("tag.readOverwrite", "");

    // TODO settings file stored in other OS place
    filename.append(getenv("HOME"));
    filename.append("/Library/Application Support/SoulSifter");
    if (!boost::filesystem::exists(filename)) {
      if (!boost::filesystem::create_directory(filename)) {
        LOG(WARNING) << "Unable to create settings directory " << filename;
      }
    }
    filename.append("/settings.json");
    if (boost::filesystem::exists(filename)) {
      // Load the JSON file into the property tree. If reading fails
      // (cannot open file, parse error), an exception is thrown.
      read_json(filename, ptree);
    }
  }

  void SoulSifterSettings::save() {
    write_json(filename, ptree);
  }

  template <typename T>
  const T SoulSifterSettings::get(const string& label) const {
    return ptree.get<T>(label);
  }
  template<> const int SoulSifterSettings::get<int>(const std::string& label) const {
    return ptree.get(label, 0);
  }
  template<> const string SoulSifterSettings::get<string>(const std::string& label) const {
    return ptree.get(label, "");
  }
  template<> const bool SoulSifterSettings::get<bool>(const std::string& label) const {
    return ptree.get(label, false);
  }

  template <typename T>
  void SoulSifterSettings::put(const string& label, const T& value) {
    ptree.put(label, value);
  }
  template void SoulSifterSettings::put<int>(const string& label, const int& value);
  template void SoulSifterSettings::put<string>(const string& label, const string& value);
  template void SoulSifterSettings::put<bool>(const string& label, const bool& value);


  string SoulSifterSettings::getString(const string& label) const { return get<std::string>(label); }
  void SoulSifterSettings::putString(const string& label, const std::string& value) { put<std::string>(label, value); }
  int SoulSifterSettings::getInt(const string& label) const { return get<int>(label); }
  void SoulSifterSettings::putInt(const string& label, const int value) { put<int>(label, value); }
  bool SoulSifterSettings::getBool(const string& label) const { return get<bool>(label); }
  void SoulSifterSettings::putBool(const string& label, const bool value) { put<bool>(label, value); }

}  // namespace soulsifter
}  // namespace dogatech
