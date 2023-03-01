#include "MusicService_wrap.h"
#include "MusicService.h"

Napi::Object MusicService::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env,
                  "MusicService",
                  {StaticMethod("cleanDirName", &MusicService::cleanDirName)});

  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("MusicService", func);
  return exports;
}

MusicService::MusicService(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<MusicService>(info) {
}

Napi::Value MusicService::cleanDirName(const Napi::CallbackInfo& info) {
  Napi::String dirName;
  if (info.Length() <= 0 || !info[0].IsString()) {
    dirName = Napi::String::New(info.Env(), "");
  } else {
    dirName = info[0].As<Napi::String>();
  }

  std::string result = dogatech::soulsifter::MusicService::cleanDirName(dirName);

  return Napi::String::New(info.Env(), result);
}
