#include <string>
#include <iostream>
#include <g3log/filesink.hpp>
#include <g3log/logmessage.hpp>

#include "SoulSifterSettings.h"

namespace dogatech {
namespace soulsifter {

class StdoutFileSink : public g3::FileSink {
public:
  StdoutFileSink(const std::string &log_prefix, const std::string &log_directory, const std::string &logger_id="g3log")
  : g3::FileSink(log_prefix, log_directory, logger_id) { }

  void log(g3::LogMessageMover msg) {
    if (msg.get()._level.value >= WARNING.value) {
      std::cerr << msg.get().toString() << std::endl;
      fileWrite(msg);
    } else {
      std::cout << msg.get().toString() << std::endl;
      if (SoulSifterSettings::getInstance().get<bool>("app.debug")) fileWrite(msg);
    }
  }
};

}  // namespace soulsifter
}  // namespace dogatech