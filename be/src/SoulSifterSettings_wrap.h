#ifndef SoulSifterSettings_wrap_h
#define SoulSifterSettings_wrap_h

#include <napi.h>
#include "SoulSifterSettings.h"

class SoulSifterSettings : public Napi::ObjectWrap<SoulSifterSettings> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  SoulSifterSettings(const Napi::CallbackInfo& info);
  ~SoulSifterSettings();

  void setWrappedValue(dogatech::soulsifter::SoulSifterSettings* v, bool own);
  dogatech::soulsifter::SoulSifterSettings* getWrappedValue() const { return soulsiftersettings; }

 private:
  // Unable to process getInstance
  Napi::Value save(const Napi::CallbackInfo& info);
  // Unable to process get
  // Unable to process put
  Napi::Value getString(const Napi::CallbackInfo& info);
  Napi::Value setString(const Napi::CallbackInfo& info);
  Napi::Value getInt(const Napi::CallbackInfo& info);
  Napi::Value setInt(const Napi::CallbackInfo& info);
  Napi::Value getBool(const Napi::CallbackInfo& info);
  Napi::Value setBool(const Napi::CallbackInfo& info);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::SoulSifterSettings* soulsiftersettings;
  bool ownWrappedObject;
};

#endif
