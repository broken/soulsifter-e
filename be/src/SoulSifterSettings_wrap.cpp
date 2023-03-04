#include <napi.h>
#include "SoulSifterSettings_wrap.h"

Napi::FunctionReference* SoulSifterSettings::constructor = nullptr;

SoulSifterSettings::~SoulSifterSettings() { if (ownWrappedObject) delete soulsiftersettings; };

void SoulSifterSettings::setWrappedValue(dogatech::soulsifter::SoulSifterSettings* v, bool own) {
  if (ownWrappedObject)
    delete soulsiftersettings;
  soulsiftersettings = v;
  ownWrappedObject = own;
}

Napi::Object SoulSifterSettings::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "SoulSifterSettings", {
    // Unable to process getInstance
    InstanceMethod<&SoulSifterSettings::save>("save"),
    // Unable to process get
    // Unable to process put
    InstanceMethod<&SoulSifterSettings::getString>("getString"),
    InstanceMethod<&SoulSifterSettings::setString>("setString"),
    InstanceMethod<&SoulSifterSettings::getInt>("getInt"),
    InstanceMethod<&SoulSifterSettings::setInt>("setInt"),
    InstanceMethod<&SoulSifterSettings::getBool>("getBool"),
    InstanceMethod<&SoulSifterSettings::setBool>("setBool"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("SoulSifterSettings", func);
  return exports;
}

Napi::Object SoulSifterSettings::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = SoulSifterSettings::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

SoulSifterSettings::SoulSifterSettings(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SoulSifterSettings>(info), soulsiftersettings(nullptr), ownWrappedObject(false) {
  soulsiftersettings = &dogatech::soulsifter::SoulSifterSettings::getInstance();
}

Napi::Value SoulSifterSettings::save(const Napi::CallbackInfo& info) {
  SoulSifterSettings* obj = this;
  obj->soulsiftersettings->save();
}

Napi::Value SoulSifterSettings::getString(const Napi::CallbackInfo& info) {
  SoulSifterSettings* obj = this;
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  string result =  obj->soulsiftersettings->getString(a0);

  return Napi::String::New(info.Env(), result);
}

Napi::Value SoulSifterSettings::setString(const Napi::CallbackInfo& info) {
  SoulSifterSettings* obj = this;
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  std::string a1(info[1].As<Napi::String>().Utf8Value());
  obj->soulsiftersettings->setString(a0, a1);
}

Napi::Value SoulSifterSettings::getInt(const Napi::CallbackInfo& info) {
  SoulSifterSettings* obj = this;
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  int result =  obj->soulsiftersettings->getInt(a0);

  return Napi::Number::New(info.Env(), result);
}

Napi::Value SoulSifterSettings::setInt(const Napi::CallbackInfo& info) {
  SoulSifterSettings* obj = this;
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  obj->soulsiftersettings->setInt(a0, a1);
}

Napi::Value SoulSifterSettings::getBool(const Napi::CallbackInfo& info) {
  SoulSifterSettings* obj = this;
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  bool result =  obj->soulsiftersettings->getBool(a0);

  return Napi::Boolean::New(info.Env(), result);
}

Napi::Value SoulSifterSettings::setBool(const Napi::CallbackInfo& info) {
  SoulSifterSettings* obj = this;
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  bool a1(info[1].As<Napi::Boolean>().Value());
  obj->soulsiftersettings->setBool(a0, a1);
}

