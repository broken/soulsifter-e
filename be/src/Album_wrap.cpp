#include <iostream>
#include <node.h>
#include <nan.h>
#include "Album_wrap.h"
#include "Album.h"
#include "Album_wrap.h"
#include "BasicGenre.h"
#include "BasicGenre_wrap.h"
#include "ResultSetIterator.h"

Nan::Persistent<v8::Function> Album::constructor;

Album::Album() : Nan::ObjectWrap(), album(NULL), ownWrappedObject(true) {};
Album::Album(dogatech::soulsifter::Album* o) : Nan::ObjectWrap(), album(o), ownWrappedObject(true) {};
Album::~Album() { if (ownWrappedObject) delete album; };

void Album::setNwcpValue(dogatech::soulsifter::Album* v, bool own) {
  if (ownWrappedObject)
    delete album;
  album = v;
  ownWrappedObject = own;
}

void Album::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Album* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::Album* xtmp(Nan::ObjectWrap::Unwrap<Album>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::Album& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::Album(x);
  } else {
    wrappedObj = new dogatech::soulsifter::Album();
  }

  Album* obj = new Album(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> Album::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void Album::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Album").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findByCoverFilepath", findByCoverFilepath);
  Nan::SetMethod(tpl, "findByNameAndArtist", findByNameAndArtist);
  Nan::SetMethod(tpl, "findByName", findByName);
  Nan::SetMethod(tpl, "findByArtist", findByArtist);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetPrototypeMethod(tpl, "reReleaseDate", reReleaseDate);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("name").ToLocalChecked(), getName, setName);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("artist").ToLocalChecked(), getArtist, setArtist);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("coverFilepath").ToLocalChecked(), getCoverFilepath, setCoverFilepath);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("mixed").ToLocalChecked(), getMixed, setMixed);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("label").ToLocalChecked(), getLabel, setLabel);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("catalogId").ToLocalChecked(), getCatalogId, setCatalogId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("releaseDateYear").ToLocalChecked(), getReleaseDateYear, setReleaseDateYear);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("releaseDateMonth").ToLocalChecked(), getReleaseDateMonth, setReleaseDateMonth);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("releaseDateDay").ToLocalChecked(), getReleaseDateDay, setReleaseDateDay);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("basicGenreId").ToLocalChecked(), getBasicGenreId, setBasicGenreId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("basicGenre").ToLocalChecked(), getBasicGenre, setBasicGenre);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("basicGenreConst").ToLocalChecked(), getBasicGenreConst);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("Album").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void Album::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  obj->album->clear();
}

void Album::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::Album* result =
      dogatech::soulsifter::Album::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Album::NewInstance();
    Album* r = Nan::ObjectWrap::Unwrap<Album>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Album::findByCoverFilepath(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::soulsifter::Album* result =
      dogatech::soulsifter::Album::findByCoverFilepath(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Album::NewInstance();
    Album* r = Nan::ObjectWrap::Unwrap<Album>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Album::findByNameAndArtist(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  string a1(*v8::String::Utf8Value(info[1]->ToString()));
  dogatech::soulsifter::Album* result =
      dogatech::soulsifter::Album::findByNameAndArtist(a0, a1);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Album::NewInstance();
    Album* r = Nan::ObjectWrap::Unwrap<Album>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Album::findByName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::ResultSetIterator<dogatech::soulsifter::Album>* result =
      dogatech::soulsifter::Album::findByName(a0);

  vector<dogatech::soulsifter::Album*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Album* o = Nan::ObjectWrap::Unwrap<Album>(instance);
    o->album = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Album::findByArtist(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::ResultSetIterator<dogatech::soulsifter::Album>* result =
      dogatech::soulsifter::Album::findByArtist(a0);

  vector<dogatech::soulsifter::Album*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Album* o = Nan::ObjectWrap::Unwrap<Album>(instance);
    o->album = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Album::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::Album>* result =
      dogatech::soulsifter::Album::findAll();

  vector<dogatech::soulsifter::Album*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Album* o = Nan::ObjectWrap::Unwrap<Album>(instance);
    o->album = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Album::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  int result =  obj->album->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Album::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  int result =  obj->album->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Album::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  bool result =  obj->album->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void Album::reReleaseDate(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const string result =  obj->album->reReleaseDate();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_GETTER(Album::getId) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const int result =  obj->album->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Album::setId) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  int a0(value->IntegerValue());
  obj->album->setId(a0);
}

NAN_GETTER(Album::getName) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const string result =  obj->album->getName();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Album::setName) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->album->setName(a0);
}

NAN_GETTER(Album::getArtist) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const string result =  obj->album->getArtist();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Album::setArtist) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->album->setArtist(a0);
}

NAN_GETTER(Album::getCoverFilepath) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const string result =  obj->album->getCoverFilepath();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Album::setCoverFilepath) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->album->setCoverFilepath(a0);
}

NAN_GETTER(Album::getMixed) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const bool result =  obj->album->getMixed();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

NAN_SETTER(Album::setMixed) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  bool a0(value->BooleanValue());
  obj->album->setMixed(a0);
}

NAN_GETTER(Album::getLabel) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const string result =  obj->album->getLabel();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Album::setLabel) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->album->setLabel(a0);
}

NAN_GETTER(Album::getCatalogId) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const string result =  obj->album->getCatalogId();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Album::setCatalogId) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->album->setCatalogId(a0);
}

NAN_GETTER(Album::getReleaseDateYear) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const int result =  obj->album->getReleaseDateYear();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Album::setReleaseDateYear) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  int a0(value->IntegerValue());
  obj->album->setReleaseDateYear(a0);
}

NAN_GETTER(Album::getReleaseDateMonth) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const int result =  obj->album->getReleaseDateMonth();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Album::setReleaseDateMonth) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  int a0(value->IntegerValue());
  obj->album->setReleaseDateMonth(a0);
}

NAN_GETTER(Album::getReleaseDateDay) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const int result =  obj->album->getReleaseDateDay();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Album::setReleaseDateDay) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  int a0(value->IntegerValue());
  obj->album->setReleaseDateDay(a0);
}

NAN_GETTER(Album::getBasicGenreId) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  const int result =  obj->album->getBasicGenreId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Album::setBasicGenreId) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  int a0(value->IntegerValue());
  obj->album->setBasicGenreId(a0);
}

NAN_GETTER(Album::getBasicGenre) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  dogatech::soulsifter::BasicGenre* result =  obj->album->getBasicGenre();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = BasicGenre::NewInstance();
    BasicGenre* r = Nan::ObjectWrap::Unwrap<BasicGenre>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_GETTER(Album::getBasicGenreConst) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  dogatech::soulsifter::BasicGenre* result =  obj->album->getBasicGenreConst();

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = BasicGenre::NewInstance();
    BasicGenre* r = Nan::ObjectWrap::Unwrap<BasicGenre>(instance);
    r->setNwcpValue(result, false);

    info.GetReturnValue().Set(instance);
  }
}

NAN_SETTER(Album::setBasicGenre) {
  Album* obj = Nan::ObjectWrap::Unwrap<Album>(info.Holder());
  dogatech::soulsifter::BasicGenre* a0tmp(Nan::ObjectWrap::Unwrap<BasicGenre>(value->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  dogatech::soulsifter::BasicGenre& a0 = *a0tmp;
  obj->album->setBasicGenre(a0);
}

