#ifndef soul_sifter_AlertsChannel_h
#define soul_sifter_AlertsChannel_h

#include <functional>
#include <string>

namespace dogatech {
namespace soulsifter {

class AlertsChannel {
 public:
  static AlertsChannel& getInstance() {
    static AlertsChannel instance;  // guaranteed to be destroyed
    // instantiated on first use
    return instance;
  }
  void registerChannelEndpoint(std::function<void(const std::string&)> alertCallback);
  void send(const std::string& message);

 private:
  std::function<void(const std::string&)> callback;
};

}  // namespace soulsifter
}  // namespace dogatech

#endif
