#include <napi.h>
#include "MusicService_wrap.h"
MusicService::~MusicService() { if (ownWrappedObject) delete musicservice; };

Napi::Object MusicService::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "MusicService", {
    StaticMethod<&MusicService::cleanDirName>("cleanDirName"),
  });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);
  exports.Set("MusicService", func);
  return exports;
}

MusicService::MusicService(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MusicService>(info) {
  Napi::Env env = info.Env();
}

Napi::Value MusicService::cleanDirName(const Napi::CallbackInfo& info) {
  Napi::String a0 = info[0].As<Napi::String>();
  string result =
      dogatech::soulsifter::MusicService::cleanDirName(a0);

  return Napi::String::New(info.Env(), result);
}

