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

Napi::Value NewSongManager::import(const Napi::CallbackInfo& info) {
  NewSongManager* obj = this;
  Napi::Array a0Array = info[0].As<Napi::Array>();
  std::vector<string> a0;
  for (uint32_t i = 0; i < a0Array.Length(); ++i) {
    Napi::Value tmp = a0Array.Get(i);
    std::string x(tmp.As<Napi::String>().Utf8Value());
    a0.push_back(x);
  }
  obj->newsongmanager->import(a0);
}

Napi::Value NewSongManager::nextSong(const Napi::CallbackInfo& info) {
  NewSongManager* obj = this;
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Song* a1(Napi::ObjectWrap<Song>::Unwrap(info[1].As<Napi::Object>())->getWrappedValue());
  bool result =  obj->newsongmanager->nextSong(a0, a1);

  return Napi::Boolean::New(info.Env(), result);
}

Napi::Value NewSongManager::coverImagePath(const Napi::CallbackInfo& info) {
  NewSongManager* obj = this;
  string result =  obj->newsongmanager->coverImagePath();

  return Napi::String::New(info.Env(), result);
}

Napi::Value NewSongManager::processSong(const Napi::CallbackInfo& info) {
  NewSongManager* obj = this;
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  bool result =  obj->newsongmanager->processSong(a0);

  return Napi::Boolean::New(info.Env(), result);
}

Napi::Value NewSongManager::trashMusicFile(const Napi::CallbackInfo& info) {
  NewSongManager* obj = this;
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  obj->newsongmanager->trashMusicFile(a0);
}

