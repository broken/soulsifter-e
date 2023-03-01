#include <iostream>
#include <node.h>
#include <nan.h>
#include "BasicGenre_wrap.h"
#include "BasicGenre.h"
#include "BasicGenre_wrap.h"
#include "ResultSetIterator.h"

Nan::Persistent<v8::Function> BasicGenre::constructor;

BasicGenre::BasicGenre() : Nan::ObjectWrap(), basicgenre(NULL), ownWrappedObject(true) {};
BasicGenre::BasicGenre(dogatech::soulsifter::BasicGenre* o) : Nan::ObjectWrap(), basicgenre(o), ownWrappedObject(true) {};
BasicGenre::~BasicGenre() { if (ownWrappedObject) delete basicgenre; };

void BasicGenre::setNwcpValue(dogatech::soulsifter::BasicGenre* v, bool own) {
  if (ownWrappedObject)
    delete basicgenre;
  basicgenre = v;
  ownWrappedObject = own;
}

void BasicGenre::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::BasicGenre* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::BasicGenre* xtmp(Nan::ObjectWrap::Unwrap<BasicGenre>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::BasicGenre& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::BasicGenre(x);
  } else {
    wrappedObj = new dogatech::soulsifter::BasicGenre();
  }

  BasicGenre* obj = new BasicGenre(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> BasicGenre::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void BasicGenre::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("BasicGenre").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findByName", findByName);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetMethod(tpl, "findByFilepath", findByFilepath);
  Nan::SetMethod(tpl, "findByArtist", findByArtist);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("name").ToLocalChecked(), getName, setName);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("BasicGenre").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void BasicGenre::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  obj->basicgenre->clear();
}

void BasicGenre::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = BasicGenre::NewInstance();
    BasicGenre* r = Nan::ObjectWrap::Unwrap<BasicGenre>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void BasicGenre::findByName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findByName(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = BasicGenre::NewInstance();
    BasicGenre* r = Nan::ObjectWrap::Unwrap<BasicGenre>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void BasicGenre::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::BasicGenre>* result =
      dogatech::soulsifter::BasicGenre::findAll();

  vector<dogatech::soulsifter::BasicGenre*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    BasicGenre* o = Nan::ObjectWrap::Unwrap<BasicGenre>(instance);
    o->basicgenre = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void BasicGenre::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  int result =  obj->basicgenre->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void BasicGenre::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  int result =  obj->basicgenre->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void BasicGenre::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  bool result =  obj->basicgenre->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void BasicGenre::findByFilepath(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findByFilepath(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = BasicGenre::NewInstance();
    BasicGenre* r = Nan::ObjectWrap::Unwrap<BasicGenre>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void BasicGenre::findByArtist(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findByArtist(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = BasicGenre::NewInstance();
    BasicGenre* r = Nan::ObjectWrap::Unwrap<BasicGenre>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

NAN_GETTER(BasicGenre::getId) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  const int result =  obj->basicgenre->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(BasicGenre::setId) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  int a0(value->IntegerValue());
  obj->basicgenre->setId(a0);
}

NAN_GETTER(BasicGenre::getName) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  const string result =  obj->basicgenre->getName();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(BasicGenre::setName) {
  BasicGenre* obj = Nan::ObjectWrap::Unwrap<BasicGenre>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->basicgenre->setName(a0);
}

