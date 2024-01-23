#include <napi.h>
#include "MusicService_wrap.h"
#include "Album.h"
#include "Album_wrap.h"

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
    StaticMethod<&MusicService::updateAlbumCover>("updateAlbumCover"),
    StaticMethod<&MusicService::moveAlbum>("moveAlbum"),
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

Napi::Value MusicService::updateAlbumCover(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 3) {
    Napi::TypeError::New(env, "Expected at least 2 arguments.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Album* a1(Napi::ObjectWrap<Album>::Unwrap(info[1].As<Napi::Object>())->getWrappedValue());
  if (!info[2].IsFunction()) {
    Napi::TypeError::New(env, "TypeError: Function expected (for info[2])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Function a2Fn = info[2].As<Napi::Function>();
  auto a2 = [&env, &a2Fn](std::string p0) {
    a2Fn.Call(env.Global(), {Napi::String::New(env, p0)});
  };
  bool result =
      dogatech::soulsifter::MusicService::updateAlbumCover(a0, a1, a2);

  return Napi::Boolean::New(env, result);
}

Napi::Value MusicService::moveAlbum(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Album* a0(Napi::ObjectWrap<Album>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  if (!info[1].IsFunction()) {
    Napi::TypeError::New(env, "TypeError: Function expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Function a1Fn = info[1].As<Napi::Function>();
  auto a1 = [&env, &a1Fn](std::string p0) {
    a1Fn.Call(env.Global(), {Napi::String::New(env, p0)});
  };
  bool result =
      dogatech::soulsifter::MusicService::moveAlbum(a0, a1);

  return Napi::Boolean::New(env, result);
}

