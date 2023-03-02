#include <napi.h>
#include "AlbumPart_wrap.h"
#include "Album.h"
#include "AlbumPart.h"
#include "AlbumPart_wrap.h"
#include "Album_wrap.h"
#include "ResultSetIterator.h"

AlbumPart::~AlbumPart() { if (ownWrappedObject) delete albumpart; };

void AlbumPart::setWrappedValue(dogatech::soulsifter::AlbumPart* v, bool own) {
  if (ownWrappedObject)
    delete albumpart;
  albumpart = v;
  ownWrappedObject = own;
}

Napi::Object AlbumPart::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "AlbumPart", {
    InstanceMethod<&AlbumPart::clear>("clear"),
    StaticMethod<&AlbumPart::findById>("findById"),
    StaticMethod<&AlbumPart::findByPosAndAlbumId>("findByPosAndAlbumId"),
    StaticMethod<&AlbumPart::findByPos>("findByPos"),
    StaticMethod<&AlbumPart::findByAlbumId>("findByAlbumId"),
    StaticMethod<&AlbumPart::findAll>("findAll"),
    InstanceMethod<&AlbumPart::update>("update"),
    InstanceMethod<&AlbumPart::save>("save"),
    InstanceMethod<&AlbumPart::sync>("sync"),
    InstanceAccessor<&AlbumPart::getId, &AlbumPart::setId>("id"),
    InstanceAccessor<&AlbumPart::getPos, &AlbumPart::setPos>("pos"),
    InstanceAccessor<&AlbumPart::getName, &AlbumPart::setName>("name"),
    InstanceAccessor<&AlbumPart::getAlbumId, &AlbumPart::setAlbumId>("albumId"),
    InstanceAccessor<&AlbumPart::getAlbum, &AlbumPart::setAlbum>("album"),
    InstanceAccessor<&AlbumPart::getAlbumConst>("albumConst"),
  });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("AlbumPart", func);
  return exports;
}

Napi::Object AlbumPart::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

AlbumPart::AlbumPart(const Napi::CallbackInfo& info) : Napi::ObjectWrap<AlbumPart>(info), albumpart(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::AlbumPart* x = Napi::ObjectWrap::Unwrap<AlbumPart>(info[0].As<Napi::Object>())->getWrappedObject();
    albumpart = new dogatech::soulsifter::AlbumPart(*x);
  } else {
    albumpart = new dogatech::soulsifter::AlbumPart();
  }
}

Napi::Value AlbumPart::clear(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  obj->albumpart->clear();
}

Napi::Value AlbumPart::findById(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::AlbumPart* result =
      dogatech::soulsifter::AlbumPart::findById(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = AlbumPart::NewInstance(info.Env());
    AlbumPart* r = Napi::ObjectWrap<AlbumPart>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value AlbumPart::findByPosAndAlbumId(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::AlbumPart* result =
      dogatech::soulsifter::AlbumPart::findByPosAndAlbumId(a0, a1);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = AlbumPart::NewInstance(info.Env());
    AlbumPart* r = Napi::ObjectWrap<AlbumPart>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value AlbumPart::findByPos(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::AlbumPart>* result =
      dogatech::soulsifter::AlbumPart::findByPos(a0);

  vector<dogatech::soulsifter::AlbumPart*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = AlbumPart::NewInstance(info.Env());
    AlbumPart* r = Napi::ObjectWrap<AlbumPart>::Unwrap(instance);
    r->setWrappedValue((*v)[i]);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value AlbumPart::findByAlbumId(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::AlbumPart>* result =
      dogatech::soulsifter::AlbumPart::findByAlbumId(a0);

  vector<dogatech::soulsifter::AlbumPart*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = AlbumPart::NewInstance(info.Env());
    AlbumPart* r = Napi::ObjectWrap<AlbumPart>::Unwrap(instance);
    r->setWrappedValue((*v)[i]);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value AlbumPart::findAll(const Napi::CallbackInfo& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::AlbumPart>* result =
      dogatech::soulsifter::AlbumPart::findAll();

  vector<dogatech::soulsifter::AlbumPart*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = AlbumPart::NewInstance(info.Env());
    AlbumPart* r = Napi::ObjectWrap<AlbumPart>::Unwrap(instance);
    r->setWrappedValue((*v)[i]);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value AlbumPart::update(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  int result =  obj->albumpart->update();

  return Napi::Number::New(info.Env(), result));
}

Napi::Value AlbumPart::save(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  int result =  obj->albumpart->save();

  return Napi::Number::New(info.Env(), result));
}

Napi::Value AlbumPart::sync(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  bool result =  obj->albumpart->sync();

  return Napi::Boolean::New(info.Env(), result));
}

Napi::Value AlbumPart::getId(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  const int result =  obj->albumpart->getId();

  return Napi::Number::New(info.Env(), result));
}

void AlbumPart::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  AlbumPart* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->albumpart->setId(a0);
}

Napi::Value AlbumPart::getPos(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  const string result =  obj->albumpart->getPos();

  return Napi::String::New(info.Env(), result);
}

void AlbumPart::setPos(const Napi::CallbackInfo& info, const Napi::Value &value) {
  AlbumPart* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->albumpart->setPos(a0);
}

Napi::Value AlbumPart::getName(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  const string result =  obj->albumpart->getName();

  return Napi::String::New(info.Env(), result);
}

void AlbumPart::setName(const Napi::CallbackInfo& info, const Napi::Value &value) {
  AlbumPart* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->albumpart->setName(a0);
}

Napi::Value AlbumPart::getAlbumId(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  const int result =  obj->albumpart->getAlbumId();

  return Napi::Number::New(info.Env(), result));
}

void AlbumPart::setAlbumId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  AlbumPart* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->albumpart->setAlbumId(a0);
}

Napi::Value AlbumPart::getAlbum(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  dogatech::soulsifter::Album* result =  obj->albumpart->getAlbum();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value AlbumPart::getAlbumConst(const Napi::CallbackInfo& info) {
  AlbumPart* obj = this;
  dogatech::soulsifter::Album* result =  obj->albumpart->getAlbumConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void AlbumPart::setAlbum(const Napi::CallbackInfo& info, const Napi::Value &value) {
  AlbumPart* obj = this;
  dogatech::soulsifter::Album* a0tmp(Napi::ObjectWrap<Album>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Album& a0 = *a0tmp;
  obj->albumpart->setAlbum(a0);
}

