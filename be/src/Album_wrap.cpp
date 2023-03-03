#include <napi.h>
#include "Album_wrap.h"
#include "Album.h"
#include "Album_wrap.h"
#include "BasicGenre.h"
#include "BasicGenre_wrap.h"
#include "ResultSetIterator.h"

Napi::FunctionReference* Album::constructor = nullptr;

Album::~Album() { if (ownWrappedObject) delete album; };

void Album::setWrappedValue(dogatech::soulsifter::Album* v, bool own) {
  if (ownWrappedObject)
    delete album;
  album = v;
  ownWrappedObject = own;
}

Napi::Object Album::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Album", {
    InstanceMethod<&Album::clear>("clear"),
    StaticMethod<&Album::findById>("findById"),
    StaticMethod<&Album::findByCoverFilepath>("findByCoverFilepath"),
    StaticMethod<&Album::findByNameAndArtist>("findByNameAndArtist"),
    StaticMethod<&Album::findByName>("findByName"),
    StaticMethod<&Album::findByArtist>("findByArtist"),
    StaticMethod<&Album::findAll>("findAll"),
    InstanceMethod<&Album::update>("update"),
    InstanceMethod<&Album::save>("save"),
    InstanceMethod<&Album::sync>("sync"),
    InstanceMethod<&Album::reReleaseDate>("reReleaseDate"),
    InstanceAccessor<&Album::getId, &Album::setId>("id"),
    InstanceAccessor<&Album::getName, &Album::setName>("name"),
    InstanceAccessor<&Album::getArtist, &Album::setArtist>("artist"),
    InstanceAccessor<&Album::getCoverFilepath, &Album::setCoverFilepath>("coverFilepath"),
    InstanceAccessor<&Album::getMixed, &Album::setMixed>("mixed"),
    InstanceAccessor<&Album::getLabel, &Album::setLabel>("label"),
    InstanceAccessor<&Album::getCatalogId, &Album::setCatalogId>("catalogId"),
    InstanceAccessor<&Album::getReleaseDateYear, &Album::setReleaseDateYear>("releaseDateYear"),
    InstanceAccessor<&Album::getReleaseDateMonth, &Album::setReleaseDateMonth>("releaseDateMonth"),
    InstanceAccessor<&Album::getReleaseDateDay, &Album::setReleaseDateDay>("releaseDateDay"),
    InstanceAccessor<&Album::getBasicGenreId, &Album::setBasicGenreId>("basicGenreId"),
    InstanceAccessor<&Album::getBasicGenre, &Album::setBasicGenre>("basicGenre"),
    InstanceAccessor<&Album::getBasicGenreConst>("basicGenreConst"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("Album", func);
  return exports;
}

Napi::Object Album::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = Album::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

Album::Album(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Album>(info), album(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::Album* x = Napi::ObjectWrap<Album>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue();
    album = new dogatech::soulsifter::Album(*x);
  } else {
    album = new dogatech::soulsifter::Album();
  }
}

Napi::Value Album::clear(const Napi::CallbackInfo& info) {
  Album* obj = this;
  obj->album->clear();
}

Napi::Value Album::findById(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::Album* result =
      dogatech::soulsifter::Album::findById(a0);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Album::findByCoverFilepath(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Album* result =
      dogatech::soulsifter::Album::findByCoverFilepath(a0);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Album::findByNameAndArtist(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  std::string a1(info[1].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Album* result =
      dogatech::soulsifter::Album::findByNameAndArtist(a0, a1);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Album::findByName(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::Album>* result =
      dogatech::soulsifter::Album::findByName(a0);

  vector<dogatech::soulsifter::Album*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Album::findByArtist(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::Album>* result =
      dogatech::soulsifter::Album::findByArtist(a0);

  vector<dogatech::soulsifter::Album*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Album::findAll(const Napi::CallbackInfo& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::Album>* result =
      dogatech::soulsifter::Album::findAll();

  vector<dogatech::soulsifter::Album*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Album::NewInstance(info.Env());
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Album::update(const Napi::CallbackInfo& info) {
  Album* obj = this;
  int result =  obj->album->update();

  return Napi::Number::New(info.Env(), result);
}

Napi::Value Album::save(const Napi::CallbackInfo& info) {
  Album* obj = this;
  int result =  obj->album->save();

  return Napi::Number::New(info.Env(), result);
}

Napi::Value Album::sync(const Napi::CallbackInfo& info) {
  Album* obj = this;
  bool result =  obj->album->sync();

  return Napi::Boolean::New(info.Env(), result);
}

Napi::Value Album::reReleaseDate(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const string result =  obj->album->reReleaseDate();

  return Napi::String::New(info.Env(), result);
}

Napi::Value Album::getId(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const int result =  obj->album->getId();

  return Napi::Number::New(info.Env(), result);
}

void Album::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->album->setId(a0);
}

Napi::Value Album::getName(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const string result =  obj->album->getName();

  return Napi::String::New(info.Env(), result);
}

void Album::setName(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->album->setName(a0);
}

Napi::Value Album::getArtist(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const string result =  obj->album->getArtist();

  return Napi::String::New(info.Env(), result);
}

void Album::setArtist(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->album->setArtist(a0);
}

Napi::Value Album::getCoverFilepath(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const string result =  obj->album->getCoverFilepath();

  return Napi::String::New(info.Env(), result);
}

void Album::setCoverFilepath(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->album->setCoverFilepath(a0);
}

Napi::Value Album::getMixed(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const bool result =  obj->album->getMixed();

  return Napi::Boolean::New(info.Env(), result);
}

void Album::setMixed(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  bool a0(value.As<Napi::Boolean>().Value());
  obj->album->setMixed(a0);
}

Napi::Value Album::getLabel(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const string result =  obj->album->getLabel();

  return Napi::String::New(info.Env(), result);
}

void Album::setLabel(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->album->setLabel(a0);
}

Napi::Value Album::getCatalogId(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const string result =  obj->album->getCatalogId();

  return Napi::String::New(info.Env(), result);
}

void Album::setCatalogId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->album->setCatalogId(a0);
}

Napi::Value Album::getReleaseDateYear(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const int result =  obj->album->getReleaseDateYear();

  return Napi::Number::New(info.Env(), result);
}

void Album::setReleaseDateYear(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->album->setReleaseDateYear(a0);
}

Napi::Value Album::getReleaseDateMonth(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const int result =  obj->album->getReleaseDateMonth();

  return Napi::Number::New(info.Env(), result);
}

void Album::setReleaseDateMonth(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->album->setReleaseDateMonth(a0);
}

Napi::Value Album::getReleaseDateDay(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const int result =  obj->album->getReleaseDateDay();

  return Napi::Number::New(info.Env(), result);
}

void Album::setReleaseDateDay(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->album->setReleaseDateDay(a0);
}

Napi::Value Album::getBasicGenreId(const Napi::CallbackInfo& info) {
  Album* obj = this;
  const int result =  obj->album->getBasicGenreId();

  return Napi::Number::New(info.Env(), result);
}

void Album::setBasicGenreId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->album->setBasicGenreId(a0);
}

Napi::Value Album::getBasicGenre(const Napi::CallbackInfo& info) {
  Album* obj = this;
  dogatech::soulsifter::BasicGenre* result =  obj->album->getBasicGenre();

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = BasicGenre::NewInstance(info.Env());
    BasicGenre* r = Napi::ObjectWrap<BasicGenre>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Album::getBasicGenreConst(const Napi::CallbackInfo& info) {
  Album* obj = this;
  dogatech::soulsifter::BasicGenre* result =  obj->album->getBasicGenreConst();

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = BasicGenre::NewInstance(info.Env());
    BasicGenre* r = Napi::ObjectWrap<BasicGenre>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Album::setBasicGenre(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Album* obj = this;
  dogatech::soulsifter::BasicGenre* a0tmp(Napi::ObjectWrap<BasicGenre>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::BasicGenre& a0 = *a0tmp;
  obj->album->setBasicGenre(a0);
}

