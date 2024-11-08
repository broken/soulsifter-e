#include <napi.h>
#include "PlaylistEntry_wrap.h"
#include "Playlist.h"
#include "PlaylistEntry.h"
#include "PlaylistEntry_wrap.h"
#include "Playlist_wrap.h"
#include "ResultSetIterator.h"
#include "Song.h"
#include "Song_wrap.h"

Napi::FunctionReference* PlaylistEntry::constructor = nullptr;

PlaylistEntry::~PlaylistEntry() { if (ownWrappedObject) delete playlistentry; };

void PlaylistEntry::setWrappedValue(dogatech::soulsifter::PlaylistEntry* v, bool own) {
  if (ownWrappedObject)
    delete playlistentry;
  playlistentry = v;
  ownWrappedObject = own;
}

Napi::Object PlaylistEntry::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "PlaylistEntry", {
    InstanceMethod<&PlaylistEntry::clear>("clear"),
    StaticMethod<&PlaylistEntry::findById>("findById"),
    StaticMethod<&PlaylistEntry::findByPlaylistIdAndSongId>("findByPlaylistIdAndSongId"),
    StaticMethod<&PlaylistEntry::findByPlaylistId>("findByPlaylistId"),
    StaticMethod<&PlaylistEntry::findBySongId>("findBySongId"),
    StaticMethod<&PlaylistEntry::findAll>("findAll"),
    InstanceMethod<&PlaylistEntry::update>("update"),
    InstanceMethod<&PlaylistEntry::save>("save"),
    InstanceMethod<&PlaylistEntry::sync>("sync"),
    InstanceMethod<&PlaylistEntry::erase>("erase"),
    InstanceAccessor<&PlaylistEntry::getId, &PlaylistEntry::setId>("id"),
    InstanceAccessor<&PlaylistEntry::getPlaylistId, &PlaylistEntry::setPlaylistId>("playlistId"),
    InstanceAccessor<&PlaylistEntry::getPlaylist, &PlaylistEntry::setPlaylist>("playlist"),
    InstanceAccessor<&PlaylistEntry::getPlaylistConst>("playlistConst"),
    InstanceAccessor<&PlaylistEntry::getSongId, &PlaylistEntry::setSongId>("songId"),
    InstanceAccessor<&PlaylistEntry::getSong, &PlaylistEntry::setSong>("song"),
    InstanceAccessor<&PlaylistEntry::getSongConst>("songConst"),
    InstanceAccessor<&PlaylistEntry::getPosition, &PlaylistEntry::setPosition>("position"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("PlaylistEntry", func);
  return exports;
}

Napi::Object PlaylistEntry::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = PlaylistEntry::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

PlaylistEntry::PlaylistEntry(const Napi::CallbackInfo& info) : Napi::ObjectWrap<PlaylistEntry>(info), playlistentry(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::PlaylistEntry* x = Napi::ObjectWrap<PlaylistEntry>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue();
    playlistentry = new dogatech::soulsifter::PlaylistEntry(*x);
  } else {
    playlistentry = new dogatech::soulsifter::PlaylistEntry();
  }
}

void PlaylistEntry::clear(const Napi::CallbackInfo& info) {
  PlaylistEntry* obj = this;
  obj->playlistentry->clear();
}

Napi::Value PlaylistEntry::findById(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::PlaylistEntry* result =
      dogatech::soulsifter::PlaylistEntry::findById(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = PlaylistEntry::NewInstance(env);
    PlaylistEntry* r = Napi::ObjectWrap<PlaylistEntry>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value PlaylistEntry::findByPlaylistIdAndSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 2 arguments.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::PlaylistEntry* result =
      dogatech::soulsifter::PlaylistEntry::findByPlaylistIdAndSongId(a0, a1);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = PlaylistEntry::NewInstance(env);
    PlaylistEntry* r = Napi::ObjectWrap<PlaylistEntry>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value PlaylistEntry::findByPlaylistId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::PlaylistEntry>* result =
      dogatech::soulsifter::PlaylistEntry::findByPlaylistId(a0);

  vector<dogatech::soulsifter::PlaylistEntry*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = PlaylistEntry::NewInstance(env);
    PlaylistEntry* r = Napi::ObjectWrap<PlaylistEntry>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value PlaylistEntry::findBySongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::PlaylistEntry>* result =
      dogatech::soulsifter::PlaylistEntry::findBySongId(a0);

  vector<dogatech::soulsifter::PlaylistEntry*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = PlaylistEntry::NewInstance(env);
    PlaylistEntry* r = Napi::ObjectWrap<PlaylistEntry>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value PlaylistEntry::findAll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  dogatech::ResultSetIterator<dogatech::soulsifter::PlaylistEntry>* result =
      dogatech::soulsifter::PlaylistEntry::findAll();

  vector<dogatech::soulsifter::PlaylistEntry*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = PlaylistEntry::NewInstance(env);
    PlaylistEntry* r = Napi::ObjectWrap<PlaylistEntry>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value PlaylistEntry::update(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  int result =  obj->playlistentry->update();

  return Napi::Number::New(env, result);
}

Napi::Value PlaylistEntry::save(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  int result =  obj->playlistentry->save();

  return Napi::Number::New(env, result);
}

Napi::Value PlaylistEntry::sync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  bool result =  obj->playlistentry->sync();

  return Napi::Boolean::New(env, result);
}

Napi::Value PlaylistEntry::erase(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  int result =  obj->playlistentry->erase();

  return Napi::Number::New(env, result);
}

Napi::Value PlaylistEntry::getId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  int result =  obj->playlistentry->getId();

  return Napi::Number::New(env, result);
}

void PlaylistEntry::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  PlaylistEntry* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->playlistentry->setId(a0);
}

Napi::Value PlaylistEntry::getPlaylistId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  int result =  obj->playlistentry->getPlaylistId();

  return Napi::Number::New(env, result);
}

void PlaylistEntry::setPlaylistId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  PlaylistEntry* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->playlistentry->setPlaylistId(a0);
}

Napi::Value PlaylistEntry::getPlaylist(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  dogatech::soulsifter::Playlist* result =  obj->playlistentry->getPlaylist();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Playlist::NewInstance(env);
    Playlist* r = Napi::ObjectWrap<Playlist>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value PlaylistEntry::getPlaylistConst(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  dogatech::soulsifter::Playlist* result =  obj->playlistentry->getPlaylistConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Playlist::NewInstance(env);
    Playlist* r = Napi::ObjectWrap<Playlist>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void PlaylistEntry::setPlaylist(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  PlaylistEntry* obj = this;
  if (!value.IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Playlist* a0tmp(Napi::ObjectWrap<Playlist>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Playlist& a0 = *a0tmp;
  obj->playlistentry->setPlaylist(a0);
}

Napi::Value PlaylistEntry::getSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  int result =  obj->playlistentry->getSongId();

  return Napi::Number::New(env, result);
}

void PlaylistEntry::setSongId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  PlaylistEntry* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->playlistentry->setSongId(a0);
}

Napi::Value PlaylistEntry::getSong(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  dogatech::soulsifter::Song* result =  obj->playlistentry->getSong();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value PlaylistEntry::getSongConst(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  dogatech::soulsifter::Song* result =  obj->playlistentry->getSongConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void PlaylistEntry::setSong(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  PlaylistEntry* obj = this;
  if (!value.IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Song* a0tmp(Napi::ObjectWrap<Song>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->playlistentry->setSong(a0);
}

Napi::Value PlaylistEntry::getPosition(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  PlaylistEntry* obj = this;
  int result =  obj->playlistentry->getPosition();

  return Napi::Number::New(env, result);
}

void PlaylistEntry::setPosition(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  PlaylistEntry* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->playlistentry->setPosition(a0);
}

