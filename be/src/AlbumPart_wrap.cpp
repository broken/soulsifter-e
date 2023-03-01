#include <iostream>
#include <node.h>
#include <nan.h>
#include "AlbumPart_wrap.h"
#include "Album.h"
#include "AlbumPart.h"
#include "AlbumPart_wrap.h"
#include "Album_wrap.h"
#include "ResultSetIterator.h"

Nan::Persistent<v8::Function> AlbumPart::constructor;

AlbumPart::AlbumPart() : Nan::ObjectWrap(), albumpart(NULL), ownWrappedObject(true) {};
AlbumPart::AlbumPart(dogatech::soulsifter::AlbumPart* o) : Nan::ObjectWrap(), albumpart(o), ownWrappedObject(true) {};
AlbumPart::~AlbumPart() { if (ownWrappedObject) delete albumpart; };

void AlbumPart::setNwcpValue(dogatech::soulsifter::AlbumPart* v, bool own) {
  if (ownWrappedObject)
    delete albumpart;
  albumpart = v;
  ownWrappedObject = own;
}

void AlbumPart::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::AlbumPart* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::AlbumPart* xtmp(Nan::ObjectWrap::Unwrap<AlbumPart>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::AlbumPart& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::AlbumPart(x);
  } else {
    wrappedObj = new dogatech::soulsifter::AlbumPart();
  }

  AlbumPart* obj = new AlbumPart(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> AlbumPart::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void AlbumPart::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("AlbumPart").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findByPosAndAlbumId", findByPosAndAlbumId);
  Nan::SetMethod(tpl, "findByPos", findByPos);
  Nan::SetMethod(tpl, "findByAlbumId", findByAlbumId);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("pos").ToLocalChecked(), getPos, setPos);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("name").ToLocalChecked(), getName, setName);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("albumId").ToLocalChecked(), getAlbumId, setAlbumId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("album").ToLocalChecked(), getAlbum, setAlbum);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("albumConst").ToLocalChecked(), getAlbumConst);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("AlbumPart").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void AlbumPart::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  obj->albumpart->clear();
}

void AlbumPart::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::AlbumPart* result =
      dogatech::soulsifter::AlbumPart::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = AlbumPart::NewInstance();
    AlbumPart* r = Nan::ObjectWrap::Unwrap<AlbumPart>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void AlbumPart::findByPosAndAlbumId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  int a1(info[1]->IntegerValue());
  dogatech::soulsifter::AlbumPart* result =
      dogatech::soulsifter::AlbumPart::findByPosAndAlbumId(a0, a1);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = AlbumPart::NewInstance();
    AlbumPart* r = Nan::ObjectWrap::Unwrap<AlbumPart>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void AlbumPart::findByPos(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::ResultSetIterator<dogatech::soulsifter::AlbumPart>* result =
      dogatech::soulsifter::AlbumPart::findByPos(a0);

  vector<dogatech::soulsifter::AlbumPart*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    AlbumPart* o = Nan::ObjectWrap::Unwrap<AlbumPart>(instance);
    o->albumpart = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void AlbumPart::findByAlbumId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::ResultSetIterator<dogatech::soulsifter::AlbumPart>* result =
      dogatech::soulsifter::AlbumPart::findByAlbumId(a0);

  vector<dogatech::soulsifter::AlbumPart*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    AlbumPart* o = Nan::ObjectWrap::Unwrap<AlbumPart>(instance);
    o->albumpart = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void AlbumPart::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::AlbumPart>* result =
      dogatech::soulsifter::AlbumPart::findAll();

  vector<dogatech::soulsifter::AlbumPart*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    AlbumPart* o = Nan::ObjectWrap::Unwrap<AlbumPart>(instance);
    o->albumpart = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void AlbumPart::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  int result =  obj->albumpart->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void AlbumPart::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  int result =  obj->albumpart->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void AlbumPart::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  bool result =  obj->albumpart->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

NAN_GETTER(AlbumPart::getId) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  const int result =  obj->albumpart->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(AlbumPart::setId) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  int a0(value->IntegerValue());
  obj->albumpart->setId(a0);
}

NAN_GETTER(AlbumPart::getPos) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  const string result =  obj->albumpart->getPos();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(AlbumPart::setPos) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->albumpart->setPos(a0);
}

NAN_GETTER(AlbumPart::getName) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  const string result =  obj->albumpart->getName();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(AlbumPart::setName) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->albumpart->setName(a0);
}

NAN_GETTER(AlbumPart::getAlbumId) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  const int result =  obj->albumpart->getAlbumId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(AlbumPart::setAlbumId) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  int a0(value->IntegerValue());
  obj->albumpart->setAlbumId(a0);
}

NAN_GETTER(AlbumPart::getAlbum) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  dogatech::soulsifter::Album* result =  obj->albumpart->getAlbum();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Album::NewInstance();
    Album* r = Nan::ObjectWrap::Unwrap<Album>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_GETTER(AlbumPart::getAlbumConst) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  dogatech::soulsifter::Album* result =  obj->albumpart->getAlbumConst();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Album::NewInstance();
    Album* r = Nan::ObjectWrap::Unwrap<Album>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_SETTER(AlbumPart::setAlbum) {
  AlbumPart* obj = Nan::ObjectWrap::Unwrap<AlbumPart>(info.Holder());
  dogatech::soulsifter::Album* a0tmp(Nan::ObjectWrap::Unwrap<Album>(value->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  dogatech::soulsifter::Album& a0 = *a0tmp;
  obj->albumpart->setAlbum(a0);
}

