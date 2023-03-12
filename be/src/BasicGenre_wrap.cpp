#include <napi.h>
#include "BasicGenre_wrap.h"
#include "BasicGenre.h"
#include "BasicGenre_wrap.h"
#include "ResultSetIterator.h"

Napi::FunctionReference* BasicGenre::constructor = nullptr;

BasicGenre::~BasicGenre() { if (ownWrappedObject) delete basicgenre; };

void BasicGenre::setWrappedValue(dogatech::soulsifter::BasicGenre* v, bool own) {
  if (ownWrappedObject)
    delete basicgenre;
  basicgenre = v;
  ownWrappedObject = own;
}

Napi::Object BasicGenre::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "BasicGenre", {
    InstanceMethod<&BasicGenre::clear>("clear"),
    StaticMethod<&BasicGenre::findById>("findById"),
    StaticMethod<&BasicGenre::findByName>("findByName"),
    StaticMethod<&BasicGenre::findAll>("findAll"),
    InstanceMethod<&BasicGenre::update>("update"),
    InstanceMethod<&BasicGenre::save>("save"),
    InstanceMethod<&BasicGenre::sync>("sync"),
    StaticMethod<&BasicGenre::findByFilepath>("findByFilepath"),
    StaticMethod<&BasicGenre::findByArtist>("findByArtist"),
    InstanceAccessor<&BasicGenre::getId, &BasicGenre::setId>("id"),
    InstanceAccessor<&BasicGenre::getName, &BasicGenre::setName>("name"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("BasicGenre", func);
  return exports;
}

Napi::Object BasicGenre::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = BasicGenre::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

BasicGenre::BasicGenre(const Napi::CallbackInfo& info) : Napi::ObjectWrap<BasicGenre>(info), basicgenre(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::BasicGenre* x = Napi::ObjectWrap<BasicGenre>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue();
    basicgenre = new dogatech::soulsifter::BasicGenre(*x);
  } else {
    basicgenre = new dogatech::soulsifter::BasicGenre();
  }
}

void BasicGenre::clear(const Napi::CallbackInfo& info) {
  BasicGenre* obj = this;
  obj->basicgenre->clear();
}

Napi::Value BasicGenre::findById(const Napi::CallbackInfo& info) {
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(info.Env(), "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findById(a0);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = BasicGenre::NewInstance(info.Env());
    BasicGenre* r = Napi::ObjectWrap<BasicGenre>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value BasicGenre::findByName(const Napi::CallbackInfo& info) {
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(info.Env(), "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findByName(a0);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = BasicGenre::NewInstance(info.Env());
    BasicGenre* r = Napi::ObjectWrap<BasicGenre>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value BasicGenre::findAll(const Napi::CallbackInfo& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::BasicGenre>* result =
      dogatech::soulsifter::BasicGenre::findAll();

  vector<dogatech::soulsifter::BasicGenre*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = BasicGenre::NewInstance(info.Env());
    BasicGenre* r = Napi::ObjectWrap<BasicGenre>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value BasicGenre::update(const Napi::CallbackInfo& info) {
  BasicGenre* obj = this;
  int result =  obj->basicgenre->update();

  return Napi::Number::New(info.Env(), result);
}

Napi::Value BasicGenre::save(const Napi::CallbackInfo& info) {
  BasicGenre* obj = this;
  int result =  obj->basicgenre->save();

  return Napi::Number::New(info.Env(), result);
}

Napi::Value BasicGenre::sync(const Napi::CallbackInfo& info) {
  BasicGenre* obj = this;
  bool result =  obj->basicgenre->sync();

  return Napi::Boolean::New(info.Env(), result);
}

Napi::Value BasicGenre::findByFilepath(const Napi::CallbackInfo& info) {
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(info.Env(), "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findByFilepath(a0);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = BasicGenre::NewInstance(info.Env());
    BasicGenre* r = Napi::ObjectWrap<BasicGenre>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value BasicGenre::findByArtist(const Napi::CallbackInfo& info) {
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(info.Env(), "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::BasicGenre* result =
      dogatech::soulsifter::BasicGenre::findByArtist(a0);

  if (result == NULL) {
    return info.Env().Null();
  } else {
    Napi::Object instance = BasicGenre::NewInstance(info.Env());
    BasicGenre* r = Napi::ObjectWrap<BasicGenre>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value BasicGenre::getId(const Napi::CallbackInfo& info) {
  BasicGenre* obj = this;
  const int result =  obj->basicgenre->getId();

  return Napi::Number::New(info.Env(), result);
}

void BasicGenre::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return;
  }
  BasicGenre* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(info.Env(), "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->basicgenre->setId(a0);
}

Napi::Value BasicGenre::getName(const Napi::CallbackInfo& info) {
  BasicGenre* obj = this;
  const string result =  obj->basicgenre->getName();

  return Napi::String::New(info.Env(), result);
}

void BasicGenre::setName(const Napi::CallbackInfo& info, const Napi::Value &value) {
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return;
  }
  BasicGenre* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(info.Env(), "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->basicgenre->setName(a0);
}

