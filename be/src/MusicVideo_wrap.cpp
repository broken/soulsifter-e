#include <napi.h>
#include "MusicVideo_wrap.h"
#include "MusicVideo.h"
#include "MusicVideo_wrap.h"
#include "ResultSetIterator.h"

MusicVideo::~MusicVideo() { if (ownWrappedObject) delete musicvideo; };

void MusicVideo::setWrappedValue(dogatech::soulsifter::MusicVideo* v, bool own) {
  if (ownWrappedObject)
    delete musicvideo;
  musicvideo = v;
  ownWrappedObject = own;
}

Napi::Object MusicVideo::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "MusicVideo", {
    InstanceMethod<&MusicVideo::clear>("clear"),
    StaticMethod<&MusicVideo::findById>("findById"),
    StaticMethod<&MusicVideo::findAll>("findAll"),
    InstanceMethod<&MusicVideo::update>("update"),
    InstanceMethod<&MusicVideo::save>("save"),
    InstanceMethod<&MusicVideo::sync>("sync"),
    InstanceAccessor<&MusicVideo::getId, &MusicVideo::setId>("id"),
    InstanceAccessor<&MusicVideo::getFilePath, &MusicVideo::setFilePath>("filePath"),
    InstanceAccessor<&MusicVideo::getThumbnailFilePath, &MusicVideo::setThumbnailFilePath>("thumbnailFilePath"),
  });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("MusicVideo", func);
  return exports;
}

Napi::Object MusicVideo::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

MusicVideo::MusicVideo(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MusicVideo>(info), musicvideo(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::MusicVideo* x = Napi::ObjectWrap::Unwrap<MusicVideo>(info[0].As<Napi::Object>())->getWrappedObject();
    musicvideo = new dogatech::soulsifter::MusicVideo(*x);
  } else {
    musicvideo = new dogatech::soulsifter::MusicVideo();
  }
}

Napi::Value MusicVideo::clear(const Napi::CallbackInfo& info) {
  MusicVideo* obj = this;
  obj->musicvideo->clear();
}

Napi::Value MusicVideo::findById(const Napi::CallbackInfo& info) {
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::MusicVideo* result =
      dogatech::soulsifter::MusicVideo::findById(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = MusicVideo::NewInstance(info.Env());
    MusicVideo* r = Napi::ObjectWrap<MusicVideo>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value MusicVideo::findAll(const Napi::CallbackInfo& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::MusicVideo>* result =
      dogatech::soulsifter::MusicVideo::findAll();

  vector<dogatech::soulsifter::MusicVideo*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = MusicVideo::NewInstance(info.Env());
    MusicVideo* r = Napi::ObjectWrap<MusicVideo>::Unwrap(instance);
    r->setWrappedValue((*v)[i]);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value MusicVideo::update(const Napi::CallbackInfo& info) {
  MusicVideo* obj = this;
  int result =  obj->musicvideo->update();

  return Napi::Number::New(info.Env(), result));
}

Napi::Value MusicVideo::save(const Napi::CallbackInfo& info) {
  MusicVideo* obj = this;
  int result =  obj->musicvideo->save();

  return Napi::Number::New(info.Env(), result));
}

Napi::Value MusicVideo::sync(const Napi::CallbackInfo& info) {
  MusicVideo* obj = this;
  bool result =  obj->musicvideo->sync();

  return Napi::Boolean::New(info.Env(), result));
}

Napi::Value MusicVideo::getId(const Napi::CallbackInfo& info) {
  MusicVideo* obj = this;
  const int result =  obj->musicvideo->getId();

  return Napi::Number::New(info.Env(), result));
}

void MusicVideo::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  MusicVideo* obj = this;
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->musicvideo->setId(a0);
}

Napi::Value MusicVideo::getFilePath(const Napi::CallbackInfo& info) {
  MusicVideo* obj = this;
  const string result =  obj->musicvideo->getFilePath();

  return Napi::String::New(info.Env(), result);
}

void MusicVideo::setFilePath(const Napi::CallbackInfo& info, const Napi::Value &value) {
  MusicVideo* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->musicvideo->setFilePath(a0);
}

Napi::Value MusicVideo::getThumbnailFilePath(const Napi::CallbackInfo& info) {
  MusicVideo* obj = this;
  const string result =  obj->musicvideo->getThumbnailFilePath();

  return Napi::String::New(info.Env(), result);
}

void MusicVideo::setThumbnailFilePath(const Napi::CallbackInfo& info, const Napi::Value &value) {
  MusicVideo* obj = this;
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->musicvideo->setThumbnailFilePath(a0);
}

