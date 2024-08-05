#ifndef AlertsChannel_wrap_h
#define AlertsChannel_wrap_h

#include <napi.h>
#include "AlertsChannel.h"

class AlertsChannel : public Napi::ObjectWrap<AlertsChannel> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  AlertsChannel(const Napi::CallbackInfo& info);
  ~AlertsChannel();

  void setWrappedValue(dogatech::soulsifter::AlertsChannel* v, bool own);
  dogatech::soulsifter::AlertsChannel* getWrappedValue() const { return alertschannel; }

 private:
  // Unable to process getInstance
  void registerChannelEndpoint(const Napi::CallbackInfo& info);
  void send(const Napi::CallbackInfo& info);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::AlertsChannel* alertschannel;
  bool ownWrappedObject;
};

#endif
