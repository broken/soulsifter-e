#include <napi.h>
#include "Mix_wrap.h"
#include "Mix.h"
#include "Mix_wrap.h"
#include "ResultSetIterator.h"
#include "Song.h"
#include "Song_wrap.h"

Mix::~Mix() { if (ownWrappedObject) delete mix; };

void Mix::setWrappedValue(dogatech::soulsifter::Mix* v, bool own) {
  if (ownWrappedObject)
    delete mix;
  mix = v;
  ownWrappedObject = own;
}

Napi::Object Mix::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Mix", {
    InstanceMethod<&Mix::clear>("clear"),
    StaticMethod<&Mix::findById>("findById"),
    StaticMethod<&Mix::findByOutSongIdAndInSongId>("findByOutSongIdAndInSongId"),
    StaticMethod<&Mix::findByOutSongId>("findByOutSongId"),
    StaticMethod<&Mix::findByInSongId>("findByInSongId"),
    StaticMethod<&Mix::findAll>("findAll"),
    InstanceMethod<&Mix::update>("update"),
    InstanceMethod<&Mix::save>("save"),
    InstanceMethod<&Mix::sync>("sync"),
    StaticMethod<&Mix::mixoutCountForRESongId>("mixoutCountForRESongId"),
    InstanceAccessor<&Mix::getId, &Mix::setId>("id"),
    InstanceAccessor<&Mix::getOutSongId, &Mix::setOutSongId>("outSongId"),
    InstanceAccessor<&Mix::getOutSong, &Mix::setOutSong>("outSong"),
    InstanceAccessor<&Mix::getOutSongConst>("outSongConst"),
    InstanceAccessor<&Mix::getInSongId, &Mix::setInSongId>("inSongId"),
    InstanceAccessor<&Mix::getInSong, &Mix::setInSong>("inSong"),
    InstanceAccessor<&Mix::getInSongConst>("inSongConst"),
    InstanceAccessor<&Mix::getBpmDiff, &Mix::setBpmDiff>("bpmDiff"),
    InstanceAccessor<&Mix::getRating, &Mix::setRating>("rating"),
    InstanceAccessor<&Mix::getComments, &Mix::setComments>("comments"),
    InstanceAccessor<&Mix::getAddon, &Mix::setAddon>("addon"),
  });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("Mix", func);
  return exports;
}

Napi::Object Mix::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

Mix::Mix(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Mix>(info), mix(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::Mix* x = Napi::ObjectWrap<Mix>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue();
    mix = new dogatech::soulsifter::Mix(*x);
  } else {
    mix = new dogatech::soulsifter::Mix();
  }
}

Napi::Value Mix::clear(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  obj->mix->clear();
}

Napi::Value Mix::findById(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::Mix* result =
      dogatech::soulsifter::Mix::findById(a0);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Mix::NewInstance(info.Env());
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Mix::findByOutSongIdAndInSongId(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::Mix* result =
      dogatech::soulsifter::Mix::findByOutSongIdAndInSongId(a0, a1);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Mix::NewInstance(info.Env());
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Mix::findByOutSongId(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findByOutSongId(a0);

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Mix::NewInstance(info.Env());
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Mix::findByInSongId(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findByInSongId(a0);

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Mix::NewInstance(info.Env());
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Mix::findAll(const Napi::CallbackInfo& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findAll();

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Mix::NewInstance(info.Env());
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Mix::update(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  int result =  obj->mix->update();

  return Napi::Number::New(info.Env(), result);
}

Napi::Value Mix::save(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  int result =  obj->mix->save();

  return Napi::Number::New(info.Env(), result);
}

Napi::Value Mix::sync(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  bool result =  obj->mix->sync();

  return Napi::Boolean::New(info.Env(), result);
}

Napi::Value Mix::mixoutCountForRESongId(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  int result =
      dogatech::soulsifter::Mix::mixoutCountForRESongId(a0);

  return Napi::Number::New(info.Env(), result);
}

Napi::Value Mix::getId(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  const int result =  obj->mix->getId();

  return Napi::Number::New(info.Env(), result);
}

void Mix::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setId(a0);
}

Napi::Value Mix::getOutSongId(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  const int result =  obj->mix->getOutSongId();

  return Napi::Number::New(info.Env(), result);
}

void Mix::setOutSongId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setOutSongId(a0);
}

Napi::Value Mix::getOutSong(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getOutSong();

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Song::NewInstance(info.Env());
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Mix::getOutSongConst(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getOutSongConst();

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Song::NewInstance(info.Env());
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Mix::setOutSong(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  dogatech::soulsifter::Song* a0tmp(Napi::ObjectWrap<Song>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->mix->setOutSong(a0);
}

Napi::Value Mix::getInSongId(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  const int result =  obj->mix->getInSongId();

  return Napi::Number::New(info.Env(), result);
}

void Mix::setInSongId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setInSongId(a0);
}

Napi::Value Mix::getInSong(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getInSong();

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Song::NewInstance(info.Env());
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Mix::getInSongConst(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getInSongConst();

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = Song::NewInstance(info.Env());
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Mix::setInSong(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  dogatech::soulsifter::Song* a0tmp(Napi::ObjectWrap<Song>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->mix->setInSong(a0);
}

Napi::Value Mix::getBpmDiff(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  const string result =  obj->mix->getBpmDiff();

  return Napi::String::New(info.Env(), result);
}

void Mix::setBpmDiff(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->mix->setBpmDiff(a0);
}

Napi::Value Mix::getRating(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  const int result =  obj->mix->getRating();

  return Napi::Number::New(info.Env(), result);
}

void Mix::setRating(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setRating(a0);
}

Napi::Value Mix::getComments(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  const string result =  obj->mix->getComments();

  return Napi::String::New(info.Env(), result);
}

void Mix::setComments(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->mix->setComments(a0);
}

Napi::Value Mix::getAddon(const Napi::CallbackInfo& info) {
  Mix* obj = this;
  const bool result =  obj->mix->getAddon();

  return Napi::Boolean::New(info.Env(), result);
}

void Mix::setAddon(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Mix* obj = this;
  bool a0(value.As<Napi::Boolean>().Value());
  obj->mix->setAddon(a0);
}

