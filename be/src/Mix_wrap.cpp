#include <iostream>
#include <node.h>
#include <nan.h>
#include "Mix_wrap.h"
#include "Mix.h"
#include "Mix_wrap.h"
#include "ResultSetIterator.h"
#include "Song.h"
#include "Song_wrap.h"

Nan::Persistent<v8::Function> Mix::constructor;

Mix::Mix() : Nan::ObjectWrap(), mix(NULL), ownWrappedObject(true) {};
Mix::Mix(dogatech::soulsifter::Mix* o) : Nan::ObjectWrap(), mix(o), ownWrappedObject(true) {};
Mix::~Mix() { if (ownWrappedObject) delete mix; };

void Mix::setNwcpValue(dogatech::soulsifter::Mix* v, bool own) {
  if (ownWrappedObject)
    delete mix;
  mix = v;
  ownWrappedObject = own;
}

void Mix::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Mix* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::Mix* xtmp(Nan::ObjectWrap::Unwrap<Mix>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::Mix& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::Mix(x);
  } else {
    wrappedObj = new dogatech::soulsifter::Mix();
  }

  Mix* obj = new Mix(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> Mix::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void Mix::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Mix").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findByOutSongIdAndInSongId", findByOutSongIdAndInSongId);
  Nan::SetMethod(tpl, "findByOutSongId", findByOutSongId);
  Nan::SetMethod(tpl, "findByInSongId", findByInSongId);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetMethod(tpl, "mixoutCountForRESongId", mixoutCountForRESongId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("outSongId").ToLocalChecked(), getOutSongId, setOutSongId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("outSong").ToLocalChecked(), getOutSong, setOutSong);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("outSongConst").ToLocalChecked(), getOutSongConst);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("inSongId").ToLocalChecked(), getInSongId, setInSongId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("inSong").ToLocalChecked(), getInSong, setInSong);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("inSongConst").ToLocalChecked(), getInSongConst);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("bpmDiff").ToLocalChecked(), getBpmDiff, setBpmDiff);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("rating").ToLocalChecked(), getRating, setRating);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("comments").ToLocalChecked(), getComments, setComments);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("addon").ToLocalChecked(), getAddon, setAddon);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("Mix").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void Mix::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  obj->mix->clear();
}

void Mix::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::Mix* result =
      dogatech::soulsifter::Mix::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Mix::NewInstance();
    Mix* r = Nan::ObjectWrap::Unwrap<Mix>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Mix::findByOutSongIdAndInSongId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  int a1(info[1]->IntegerValue());
  dogatech::soulsifter::Mix* result =
      dogatech::soulsifter::Mix::findByOutSongIdAndInSongId(a0, a1);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Mix::NewInstance();
    Mix* r = Nan::ObjectWrap::Unwrap<Mix>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Mix::findByOutSongId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findByOutSongId(a0);

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Mix* o = Nan::ObjectWrap::Unwrap<Mix>(instance);
    o->mix = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Mix::findByInSongId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findByInSongId(a0);

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Mix* o = Nan::ObjectWrap::Unwrap<Mix>(instance);
    o->mix = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Mix::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findAll();

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Mix* o = Nan::ObjectWrap::Unwrap<Mix>(instance);
    o->mix = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Mix::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  int result =  obj->mix->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Mix::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  int result =  obj->mix->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Mix::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  bool result =  obj->mix->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void Mix::mixoutCountForRESongId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  int result =
      dogatech::soulsifter::Mix::mixoutCountForRESongId(a0);

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_GETTER(Mix::getId) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  const int result =  obj->mix->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Mix::setId) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  int a0(value->IntegerValue());
  obj->mix->setId(a0);
}

NAN_GETTER(Mix::getOutSongId) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  const int result =  obj->mix->getOutSongId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Mix::setOutSongId) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  int a0(value->IntegerValue());
  obj->mix->setOutSongId(a0);
}

NAN_GETTER(Mix::getOutSong) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  dogatech::soulsifter::Song* result =  obj->mix->getOutSong();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Song::NewInstance();
    Song* r = Nan::ObjectWrap::Unwrap<Song>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_GETTER(Mix::getOutSongConst) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  dogatech::soulsifter::Song* result =  obj->mix->getOutSongConst();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Song::NewInstance();
    Song* r = Nan::ObjectWrap::Unwrap<Song>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_SETTER(Mix::setOutSong) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  dogatech::soulsifter::Song* a0tmp(Nan::ObjectWrap::Unwrap<Song>(value->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->mix->setOutSong(a0);
}

NAN_GETTER(Mix::getInSongId) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  const int result =  obj->mix->getInSongId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Mix::setInSongId) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  int a0(value->IntegerValue());
  obj->mix->setInSongId(a0);
}

NAN_GETTER(Mix::getInSong) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  dogatech::soulsifter::Song* result =  obj->mix->getInSong();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Song::NewInstance();
    Song* r = Nan::ObjectWrap::Unwrap<Song>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_GETTER(Mix::getInSongConst) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  dogatech::soulsifter::Song* result =  obj->mix->getInSongConst();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Song::NewInstance();
    Song* r = Nan::ObjectWrap::Unwrap<Song>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_SETTER(Mix::setInSong) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  dogatech::soulsifter::Song* a0tmp(Nan::ObjectWrap::Unwrap<Song>(value->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->mix->setInSong(a0);
}

NAN_GETTER(Mix::getBpmDiff) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  const string result =  obj->mix->getBpmDiff();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Mix::setBpmDiff) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->mix->setBpmDiff(a0);
}

NAN_GETTER(Mix::getRating) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  const int result =  obj->mix->getRating();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Mix::setRating) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  int a0(value->IntegerValue());
  obj->mix->setRating(a0);
}

NAN_GETTER(Mix::getComments) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  const string result =  obj->mix->getComments();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Mix::setComments) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->mix->setComments(a0);
}

NAN_GETTER(Mix::getAddon) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  const bool result =  obj->mix->getAddon();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

NAN_SETTER(Mix::setAddon) {
  Mix* obj = Nan::ObjectWrap::Unwrap<Mix>(info.Holder());
  bool a0(value->BooleanValue());
  obj->mix->setAddon(a0);
}

