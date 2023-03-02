#include <napi.h>
#include "MusicVideoService_wrap.h"
#include "MusicVideo.h"
#include "MusicVideo_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

MusicVideoService::~MusicVideoService() { if (ownWrappedObject) delete musicvideoservice; };

void MusicVideoService::setWrappedValue(dogatech::soulsifter::MusicVideoService* v, bool own) {
  if (ownWrappedObject)
    delete musicvideoservice;
  musicvideoservice = v;
  ownWrappedObject = own;
}

Napi::Object MusicVideoService::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "MusicVideoService", {
    StaticMethod<&MusicVideoService::associateYouTubeVideo>("associateYouTubeVideo"),
    StaticMethod<&MusicVideoService::downloadAudio>("downloadAudio"),
  });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("MusicVideoService", func);
  return exports;
}

Napi::Object MusicVideoService::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

MusicVideoService::MusicVideoService(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MusicVideoService>(info), musicvideoservice(nullptr), ownWrappedObject(true) {
  musicvideoservice = new dogatech::soulsifter::MusicVideoService();
}

Napi::Value MusicVideoService::associateYouTubeVideo(const Napi::CallbackInfo& info) {
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  std::string a1(info[1].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::MusicVideo* result =
      dogatech::soulsifter::MusicVideoService::associateYouTubeVideo(a0, a1);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = MusicVideo::NewInstance(info.Env());
    MusicVideo* r = Napi::ObjectWrap<MusicVideo>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value MusicVideoService::downloadAudio(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  std::vector<string> result =
      dogatech::soulsifter::MusicVideoService::downloadAudio(a0);

  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    a.Set(i, Napi::String::New(info.Env(), result[i]);
  }
  return a;
}

