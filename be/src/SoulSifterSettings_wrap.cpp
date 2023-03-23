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
    InstanceMethod<&SoulSifterSettings::putString>("putString"),
    InstanceMethod<&SoulSifterSettings::getInt>("getInt"),
    InstanceMethod<&SoulSifterSettings::putInt>("putInt"),
    InstanceMethod<&SoulSifterSettings::getBool>("getBool"),
    InstanceMethod<&SoulSifterSettings::putBool>("putBool"),
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

void SoulSifterSettings::save(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  SoulSifterSettings* obj = this;
  obj->soulsiftersettings->save();
}

Napi::Value SoulSifterSettings::getString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return env.Null();
  }
  SoulSifterSettings* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  string result =  obj->soulsiftersettings->getString(a0);

  return Napi::String::New(env, result);
}

void SoulSifterSettings::putString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 2 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return;
  }
  SoulSifterSettings* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[1])").ThrowAsJavaScriptException();
    return;
  }
  std::string a1(info[1].As<Napi::String>().Utf8Value());
  obj->soulsiftersettings->putString(a0, a1);
}

Napi::Value SoulSifterSettings::getInt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return env.Null();
  }
  SoulSifterSettings* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  int result =  obj->soulsiftersettings->getInt(a0);

  return Napi::Number::New(env, result);
}

void SoulSifterSettings::putInt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 2 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return;
  }
  SoulSifterSettings* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[1])").ThrowAsJavaScriptException();
    return;
  }
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  obj->soulsiftersettings->putInt(a0, a1);
}

Napi::Value SoulSifterSettings::getBool(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return env.Null();
  }
  SoulSifterSettings* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  bool result =  obj->soulsiftersettings->getBool(a0);

  return Napi::Boolean::New(env, result);
}

void SoulSifterSettings::putBool(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 2 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return;
  }
  SoulSifterSettings* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  if (!info[1].IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for info[1])").ThrowAsJavaScriptException();
    return;
  }
  bool a1(info[1].As<Napi::Boolean>().Value());
  obj->soulsiftersettings->putBool(a0, a1);
}

