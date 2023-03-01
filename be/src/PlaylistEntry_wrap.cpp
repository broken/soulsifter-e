#include <iostream>
#include <node.h>
#include <nan.h>
#include "PlaylistEntry_wrap.h"
#include "Playlist.h"
#include "PlaylistEntry.h"
#include "PlaylistEntry_wrap.h"
#include "Playlist_wrap.h"
#include "ResultSetIterator.h"
#include "Song.h"
#include "Song_wrap.h"

Nan::Persistent<v8::Function> PlaylistEntry::constructor;

PlaylistEntry::PlaylistEntry() : Nan::ObjectWrap(), playlistentry(NULL), ownWrappedObject(true) {};
PlaylistEntry::PlaylistEntry(dogatech::soulsifter::PlaylistEntry* o) : Nan::ObjectWrap(), playlistentry(o), ownWrappedObject(true) {};
PlaylistEntry::~PlaylistEntry() { if (ownWrappedObject) delete playlistentry; };

void PlaylistEntry::setNwcpValue(dogatech::soulsifter::PlaylistEntry* v, bool own) {
  if (ownWrappedObject)
    delete playlistentry;
  playlistentry = v;
  ownWrappedObject = own;
}

void PlaylistEntry::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::PlaylistEntry* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::PlaylistEntry* xtmp(Nan::ObjectWrap::Unwrap<PlaylistEntry>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::PlaylistEntry& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::PlaylistEntry(x);
  } else {
    wrappedObj = new dogatech::soulsifter::PlaylistEntry();
  }

  PlaylistEntry* obj = new PlaylistEntry(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> PlaylistEntry::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void PlaylistEntry::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("PlaylistEntry").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findByPlaylistIdAndSongId", findByPlaylistIdAndSongId);
  Nan::SetMethod(tpl, "findByPlaylistId", findByPlaylistId);
  Nan::SetMethod(tpl, "findBySongId", findBySongId);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetPrototypeMethod(tpl, "erase", erase);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("playlistId").ToLocalChecked(), getPlaylistId, setPlaylistId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("playlist").ToLocalChecked(), getPlaylist, setPlaylist);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("playlistConst").ToLocalChecked(), getPlaylistConst);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("songId").ToLocalChecked(), getSongId, setSongId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("song").ToLocalChecked(), getSong, setSong);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("songConst").ToLocalChecked(), getSongConst);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("position").ToLocalChecked(), getPosition, setPosition);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("time").ToLocalChecked(), getTime, setTime);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("PlaylistEntry").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void PlaylistEntry::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  obj->playlistentry->clear();
}

void PlaylistEntry::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::PlaylistEntry* result =
      dogatech::soulsifter::PlaylistEntry::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = PlaylistEntry::NewInstance();
    PlaylistEntry* r = Nan::ObjectWrap::Unwrap<PlaylistEntry>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void PlaylistEntry::findByPlaylistIdAndSongId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  int a1(info[1]->IntegerValue());
  dogatech::soulsifter::PlaylistEntry* result =
      dogatech::soulsifter::PlaylistEntry::findByPlaylistIdAndSongId(a0, a1);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = PlaylistEntry::NewInstance();
    PlaylistEntry* r = Nan::ObjectWrap::Unwrap<PlaylistEntry>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void PlaylistEntry::findByPlaylistId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::ResultSetIterator<dogatech::soulsifter::PlaylistEntry>* result =
      dogatech::soulsifter::PlaylistEntry::findByPlaylistId(a0);

  vector<dogatech::soulsifter::PlaylistEntry*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    PlaylistEntry* o = Nan::ObjectWrap::Unwrap<PlaylistEntry>(instance);
    o->playlistentry = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void PlaylistEntry::findBySongId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::ResultSetIterator<dogatech::soulsifter::PlaylistEntry>* result =
      dogatech::soulsifter::PlaylistEntry::findBySongId(a0);

  vector<dogatech::soulsifter::PlaylistEntry*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    PlaylistEntry* o = Nan::ObjectWrap::Unwrap<PlaylistEntry>(instance);
    o->playlistentry = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void PlaylistEntry::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::PlaylistEntry>* result =
      dogatech::soulsifter::PlaylistEntry::findAll();

  vector<dogatech::soulsifter::PlaylistEntry*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    PlaylistEntry* o = Nan::ObjectWrap::Unwrap<PlaylistEntry>(instance);
    o->playlistentry = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void PlaylistEntry::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  int result =  obj->playlistentry->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void PlaylistEntry::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  int result =  obj->playlistentry->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void PlaylistEntry::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  bool result =  obj->playlistentry->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void PlaylistEntry::erase(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  int result =  obj->playlistentry->erase();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_GETTER(PlaylistEntry::getId) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  const int result =  obj->playlistentry->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(PlaylistEntry::setId) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  int a0(value->IntegerValue());
  obj->playlistentry->setId(a0);
}

NAN_GETTER(PlaylistEntry::getPlaylistId) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  const int result =  obj->playlistentry->getPlaylistId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(PlaylistEntry::setPlaylistId) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  int a0(value->IntegerValue());
  obj->playlistentry->setPlaylistId(a0);
}

NAN_GETTER(PlaylistEntry::getPlaylist) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  dogatech::soulsifter::Playlist* result =  obj->playlistentry->getPlaylist();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Playlist::NewInstance();
    Playlist* r = Nan::ObjectWrap::Unwrap<Playlist>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_GETTER(PlaylistEntry::getPlaylistConst) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  dogatech::soulsifter::Playlist* result =  obj->playlistentry->getPlaylistConst();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Playlist::NewInstance();
    Playlist* r = Nan::ObjectWrap::Unwrap<Playlist>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_SETTER(PlaylistEntry::setPlaylist) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  dogatech::soulsifter::Playlist* a0tmp(Nan::ObjectWrap::Unwrap<Playlist>(value->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  dogatech::soulsifter::Playlist& a0 = *a0tmp;
  obj->playlistentry->setPlaylist(a0);
}

NAN_GETTER(PlaylistEntry::getSongId) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  const int result =  obj->playlistentry->getSongId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(PlaylistEntry::setSongId) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  int a0(value->IntegerValue());
  obj->playlistentry->setSongId(a0);
}

NAN_GETTER(PlaylistEntry::getSong) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  dogatech::soulsifter::Song* result =  obj->playlistentry->getSong();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Song::NewInstance();
    Song* r = Nan::ObjectWrap::Unwrap<Song>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_GETTER(PlaylistEntry::getSongConst) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  dogatech::soulsifter::Song* result =  obj->playlistentry->getSongConst();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Song::NewInstance();
    Song* r = Nan::ObjectWrap::Unwrap<Song>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_SETTER(PlaylistEntry::setSong) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  dogatech::soulsifter::Song* a0tmp(Nan::ObjectWrap::Unwrap<Song>(value->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->playlistentry->setSong(a0);
}

NAN_GETTER(PlaylistEntry::getPosition) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  const int result =  obj->playlistentry->getPosition();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(PlaylistEntry::setPosition) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  int a0(value->IntegerValue());
  obj->playlistentry->setPosition(a0);
}

NAN_GETTER(PlaylistEntry::getTime) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  const string result =  obj->playlistentry->getTime();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(PlaylistEntry::setTime) {
  PlaylistEntry* obj = Nan::ObjectWrap::Unwrap<PlaylistEntry>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->playlistentry->setTime(a0);
}

