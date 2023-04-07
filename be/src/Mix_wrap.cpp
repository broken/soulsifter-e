#include <napi.h>
#include "Mix_wrap.h"
#include "Mix.h"
#include "Mix_wrap.h"
#include "ResultSetIterator.h"
#include "Song.h"
#include "Song_wrap.h"

Napi::FunctionReference* Mix::constructor = nullptr;

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

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("Mix", func);
  return exports;
}

Napi::Object Mix::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = Mix::constructor->New({});
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

void Mix::clear(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  obj->mix->clear();
}

Napi::Value Mix::findById(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::Mix* result =
      dogatech::soulsifter::Mix::findById(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Mix::NewInstance(env);
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Mix::findByOutSongIdAndInSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 2 arguments.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::Mix* result =
      dogatech::soulsifter::Mix::findByOutSongIdAndInSongId(a0, a1);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Mix::NewInstance(env);
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Mix::findByOutSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findByOutSongId(a0);

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Mix::NewInstance(env);
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Mix::findByInSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findByInSongId(a0);

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Mix::NewInstance(env);
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Mix::findAll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  dogatech::ResultSetIterator<dogatech::soulsifter::Mix>* result =
      dogatech::soulsifter::Mix::findAll();

  vector<dogatech::soulsifter::Mix*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Mix::NewInstance(env);
    Mix* r = Napi::ObjectWrap<Mix>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Mix::update(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  int result =  obj->mix->update();

  return Napi::Number::New(env, result);
}

Napi::Value Mix::save(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  int result =  obj->mix->save();

  return Napi::Number::New(env, result);
}

Napi::Value Mix::sync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  bool result =  obj->mix->sync();

  return Napi::Boolean::New(env, result);
}

Napi::Value Mix::mixoutCountForRESongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  int result =
      dogatech::soulsifter::Mix::mixoutCountForRESongId(a0);

  return Napi::Number::New(env, result);
}

Napi::Value Mix::getId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  int result =  obj->mix->getId();

  return Napi::Number::New(env, result);
}

void Mix::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setId(a0);
}

Napi::Value Mix::getOutSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  int result =  obj->mix->getOutSongId();

  return Napi::Number::New(env, result);
}

void Mix::setOutSongId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setOutSongId(a0);
}

Napi::Value Mix::getOutSong(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getOutSong();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Mix::getOutSongConst(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getOutSongConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Mix::setOutSong(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Song* a0tmp(Napi::ObjectWrap<Song>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->mix->setOutSong(a0);
}

Napi::Value Mix::getInSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  int result =  obj->mix->getInSongId();

  return Napi::Number::New(env, result);
}

void Mix::setInSongId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setInSongId(a0);
}

Napi::Value Mix::getInSong(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getInSong();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Mix::getInSongConst(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  dogatech::soulsifter::Song* result =  obj->mix->getInSongConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Mix::setInSong(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Song* a0tmp(Napi::ObjectWrap<Song>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Song& a0 = *a0tmp;
  obj->mix->setInSong(a0);
}

Napi::Value Mix::getBpmDiff(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  const string result =  obj->mix->getBpmDiff();

  return Napi::String::New(env, result);
}

void Mix::setBpmDiff(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->mix->setBpmDiff(a0);
}

Napi::Value Mix::getRating(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  int result =  obj->mix->getRating();

  return Napi::Number::New(env, result);
}

void Mix::setRating(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->mix->setRating(a0);
}

Napi::Value Mix::getComments(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  const string result =  obj->mix->getComments();

  return Napi::String::New(env, result);
}

void Mix::setComments(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->mix->setComments(a0);
}

Napi::Value Mix::getAddon(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Mix* obj = this;
  bool result =  obj->mix->getAddon();

  return Napi::Boolean::New(env, result);
}

void Mix::setAddon(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Mix* obj = this;
  if (!value.IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  bool a0(value.As<Napi::Boolean>().Value());
  obj->mix->setAddon(a0);
}

