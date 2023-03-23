#include <napi.h>
#include "NewSongManager_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Napi::FunctionReference* NewSongManager::constructor = nullptr;

NewSongManager::~NewSongManager() { if (ownWrappedObject) delete newsongmanager; };

void NewSongManager::setWrappedValue(dogatech::soulsifter::NewSongManager* v, bool own) {
  if (ownWrappedObject)
    delete newsongmanager;
  newsongmanager = v;
  ownWrappedObject = own;
}

Napi::Object NewSongManager::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "NewSongManager", {
    InstanceMethod<&NewSongManager::import>("import"),
    InstanceMethod<&NewSongManager::nextSong>("nextSong"),
    InstanceMethod<&NewSongManager::coverImagePath>("coverImagePath"),
    InstanceMethod<&NewSongManager::processSong>("processSong"),
    InstanceMethod<&NewSongManager::trashMusicFile>("trashMusicFile"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("NewSongManager", func);
  return exports;
}

Napi::Object NewSongManager::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = NewSongManager::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

NewSongManager::NewSongManager(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NewSongManager>(info), newsongmanager(nullptr), ownWrappedObject(true) {
  newsongmanager = new dogatech::soulsifter::NewSongManager();
}

void NewSongManager::import(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return;
  }
  NewSongManager* obj = this;
  if (!info[0].IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  Napi::Array a0Array = info[0].As<Napi::Array>();
  std::vector<string> a0;
  for (uint32_t i = 0; i < a0Array.Length(); ++i) {
    if (!a0Array.Get(i).IsString()) {
      Napi::TypeError::New(env, "TypeError: String expected (for a0Array.Get(i))").ThrowAsJavaScriptException();
      return;
    }
    std::string x(a0Array.Get(i).As<Napi::String>().Utf8Value());
    a0.push_back(x);
  }
  obj->newsongmanager->import(a0);
}

Napi::Value NewSongManager::nextSong(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 2 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return env.Null();
  }
  NewSongManager* obj = this;
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Song* a1(Napi::ObjectWrap<Song>::Unwrap(info[1].As<Napi::Object>())->getWrappedValue());
  bool result =  obj->newsongmanager->nextSong(a0, a1);

  return Napi::Boolean::New(env, result);
}

Napi::Value NewSongManager::coverImagePath(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  NewSongManager* obj = this;
  string result =  obj->newsongmanager->coverImagePath();

  return Napi::String::New(env, result);
}

Napi::Value NewSongManager::processSong(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return env.Null();
  }
  NewSongManager* obj = this;
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  bool result =  obj->newsongmanager->processSong(a0);

  return Napi::Boolean::New(env, result);
}

void NewSongManager::trashMusicFile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return;
  }
  NewSongManager* obj = this;
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  obj->newsongmanager->trashMusicFile(a0);
}

