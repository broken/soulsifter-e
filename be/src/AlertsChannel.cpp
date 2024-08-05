#include "AlertsChannel.h"

#include <functional>
#include <string>

namespace dogatech {
namespace soulsifter {

void AlertsChannel::registerChannelEndpoint(std::function<void(const std::string&)> callback) {
  this->callback = callback;
}

void AlertsChannel::send(const std::string& message) {
  callback(message);
}

}  // namespace soulsifter
}  // namespace dogatech