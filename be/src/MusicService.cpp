#include "MusicService.h"

#include <string>

#include "MusicManager.h"

namespace dogatech {
namespace soulsifter {

std::string MusicService::cleanDirName(const std::string& s) {
  std::string name(s);
  std::replace(name.begin(), name.end(), '/', '-');
  std::replace(name.begin(), name.end(), ':', ' ');
  std::replace(name.begin(), name.end(), '\\', ' ');
  // boost::regex charsToRemoveRegex("[!@#$%^&*?'\";]");
  // boost::regex prefixSpacesRegex("^[ ]*");
  // boost::regex suffixSpacesRegex("[ ]*$");
  // name = boost::regex_replace(name, charsToRemoveRegex, "");
  // name = boost::regex_replace(name, prefixSpacesRegex, "");
  // name = boost::regex_replace(name, suffixSpacesRegex, "");
  // remove nonascii characters
  // name.erase(remove_if(name.begin(), name.end(), [](auto c) { return !isascii(c); }), name.end());
  return name;
}

}  // namespace soulsifter
}  // namespace dogatech
