#include <iostream>
#include <node.h>
#include <nan.h>
#include "Playlist_wrap.h"
#include "Playlist.h"
#include "Playlist_wrap.h"
#include "ResultSetIterator.h"
#include "Style.h"
#include "Style_wrap.h"

Nan::Persistent<v8::Function> Playlist::constructor;

Playlist::Playlist() : Nan::ObjectWrap(), playlist(NULL), ownWrappedObject(true) {};
Playlist::Playlist(dogatech::soulsifter::Playlist* o) : Nan::ObjectWrap(), playlist(o), ownWrappedObject(true) {};
Playlist::~Playlist() { if (ownWrappedObject) delete playlist; };

void Playlist::setNwcpValue(dogatech::soulsifter::Playlist* v, bool own) {
  if (ownWrappedObject)
    delete playlist;
  playlist = v;
  ownWrappedObject = own;
}

void Playlist::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Playlist* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::Playlist* xtmp(Nan::ObjectWrap::Unwrap<Playlist>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::Playlist& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::Playlist(x);
  } else {
    wrappedObj = new dogatech::soulsifter::Playlist();
  }

  Playlist* obj = new Playlist(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> Playlist::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void Playlist::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Playlist").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findByName", findByName);
  Nan::SetMethod(tpl, "findBySpotifyId", findBySpotifyId);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetPrototypeMethod(tpl, "erase", erase);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("name").ToLocalChecked(), getName, setName);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("query").ToLocalChecked(), getQuery, setQuery);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("gmusicId").ToLocalChecked(), getGmusicId, setGmusicId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("youtubeId").ToLocalChecked(), getYoutubeId, setYoutubeId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("spotifyId").ToLocalChecked(), getSpotifyId, setSpotifyId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("styleIds").ToLocalChecked(), getStyleIds, setStyleIds);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("styles").ToLocalChecked(), getStyles, setStyles);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("Playlist").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void Playlist::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  obj->playlist->clear();
}

void Playlist::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::Playlist* result =
      dogatech::soulsifter::Playlist::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Playlist::NewInstance();
    Playlist* r = Nan::ObjectWrap::Unwrap<Playlist>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Playlist::findByName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::soulsifter::Playlist* result =
      dogatech::soulsifter::Playlist::findByName(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Playlist::NewInstance();
    Playlist* r = Nan::ObjectWrap::Unwrap<Playlist>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Playlist::findBySpotifyId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::soulsifter::Playlist* result =
      dogatech::soulsifter::Playlist::findBySpotifyId(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Playlist::NewInstance();
    Playlist* r = Nan::ObjectWrap::Unwrap<Playlist>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Playlist::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::Playlist>* result =
      dogatech::soulsifter::Playlist::findAll();

  vector<dogatech::soulsifter::Playlist*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Playlist* o = Nan::ObjectWrap::Unwrap<Playlist>(instance);
    o->playlist = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Playlist::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  int result =  obj->playlist->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Playlist::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  int result =  obj->playlist->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Playlist::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  bool result =  obj->playlist->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void Playlist::erase(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  int result =  obj->playlist->erase();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_GETTER(Playlist::getId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const int result =  obj->playlist->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Playlist::setId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  int a0(value->IntegerValue());
  obj->playlist->setId(a0);
}

NAN_GETTER(Playlist::getName) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const string result =  obj->playlist->getName();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Playlist::setName) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->playlist->setName(a0);
}

NAN_GETTER(Playlist::getQuery) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const string result =  obj->playlist->getQuery();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Playlist::setQuery) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->playlist->setQuery(a0);
}

NAN_GETTER(Playlist::getGmusicId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const string result =  obj->playlist->getGmusicId();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Playlist::setGmusicId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->playlist->setGmusicId(a0);
}

NAN_GETTER(Playlist::getYoutubeId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const string result =  obj->playlist->getYoutubeId();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Playlist::setYoutubeId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->playlist->setYoutubeId(a0);
}

NAN_GETTER(Playlist::getSpotifyId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const string result =  obj->playlist->getSpotifyId();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Playlist::setSpotifyId) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->playlist->setSpotifyId(a0);
}

NAN_GETTER(Playlist::getStyleIds) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const std::vector<int> result =  obj->playlist->getStyleIds();

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result.size());
  for (int i = 0; i < (int) result.size(); i++) {
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), Nan::New<v8::Integer>(result[i]));
  }
  info.GetReturnValue().Set(a);
}

NAN_SETTER(Playlist::setStyleIds) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  v8::Local<v8::Array> a0Array = v8::Local<v8::Array>::Cast(value);
  std::vector<int> a0;
  for (uint32_t i = 0; i < a0Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a0Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    int x(tmp->IntegerValue());
    a0.push_back(x);
  }
  obj->playlist->setStyleIds(a0);
}

NAN_GETTER(Playlist::getStyles) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  const std::vector<dogatech::soulsifter::Style*> result =  obj->playlist->getStyles();

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result.size());
  for (int i = 0; i < (int) result.size(); i++) {
    v8::Local<v8::Object> instance = Style::NewInstance();
    Style* r = Nan::ObjectWrap::Unwrap<Style>(instance);
    r->setNwcpValue((result)[i], false);
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  info.GetReturnValue().Set(a);
}

NAN_SETTER(Playlist::setStyles) {
  Playlist* obj = Nan::ObjectWrap::Unwrap<Playlist>(info.Holder());
  v8::Local<v8::Array> a0Array = v8::Local<v8::Array>::Cast(value);
  std::vector<dogatech::soulsifter::Style*> a0;
  for (uint32_t i = 0; i < a0Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a0Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    dogatech::soulsifter::Style* x(Nan::ObjectWrap::Unwrap<Style>(tmp->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
    a0.push_back(x);
  }
  obj->playlist->setStyles(a0);
}

