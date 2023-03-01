//
// This was not auto-generated unlike the others.

#ifndef SoulSifterSettings_wrap_h
#define SoulSifterSettings_wrap_h

#include <node.h>
#include <nan.h>
#include "SoulSifterSettings.h"

class SoulSifterSettings : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::SoulSifterSettings* v, bool own);
  dogatech::soulsifter::SoulSifterSettings* getNwcpValue() const { return soulsiftersettings; }

 private:
  SoulSifterSettings();
  explicit SoulSifterSettings(dogatech::soulsifter::SoulSifterSettings* soulsiftersettings);
  ~SoulSifterSettings();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getString(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void putString(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getInt(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void putInt(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getBool(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void putBool(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::SoulSifterSettings* soulsiftersettings;
  bool ownWrappedObject;
};

#endif
