#include <napi.h>
#include "MusicService_wrap.h"

Napi::FunctionReference* MusicService::constructor = nullptr;

MusicService::~MusicService() { if (ownWrappedObject) delete musicservice; };

void MusicService::setWrappedValue(dogatech::soulsifter::MusicService* v, bool own) {
  if (ownWrappedObject)
    delete musicservice;
  musicservice = v;
  ownWrappedObject = own;
}

Napi::Object MusicService::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "MusicService", {
    StaticMethod<&MusicService::cleanDirName>("cleanDirName"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("MusicService", func);
  return exports;
}

Napi::Object MusicService::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = MusicService::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

MusicService::MusicService(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MusicService>(info), musicservice(nullptr), ownWrappedObject(true) {
  musicservice = new dogatech::soulsifter::MusicService();
}

Napi::Value MusicService::cleanDirName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  string result =
      dogatech::soulsifter::MusicService::cleanDirName(a0);

  return Napi::String::New(env, result);
}

