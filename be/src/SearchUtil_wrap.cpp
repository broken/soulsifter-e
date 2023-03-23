#include <napi.h>
#include "SearchUtil_wrap.h"
#include "Song.h"
#include "Song_wrap.h"
#include "Style.h"
#include "Style_wrap.h"

Napi::FunctionReference* SearchUtil::constructor = nullptr;

SearchUtil::~SearchUtil() { if (ownWrappedObject) delete searchutil; };

void SearchUtil::setWrappedValue(dogatech::soulsifter::SearchUtil* v, bool own) {
  if (ownWrappedObject)
    delete searchutil;
  searchutil = v;
  ownWrappedObject = own;
}

Napi::Object SearchUtil::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "SearchUtil", {
    StaticMethod<&SearchUtil::searchSongs>("searchSongs"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("SearchUtil", func);
  return exports;
}

Napi::Object SearchUtil::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = SearchUtil::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

SearchUtil::SearchUtil(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SearchUtil>(info), searchutil(nullptr), ownWrappedObject(true) {
  searchutil = new dogatech::soulsifter::SearchUtil();
}

Napi::Value SearchUtil::searchSongs(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 10) {
    Napi::TypeError::New(env, "Expected at least 6 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  if (!info[2].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[2])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a2(info[2].As<Napi::String>().Utf8Value());
  if (!info[3].IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for info[3])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Array a3Array = info[3].As<Napi::Array>();
  std::vector<dogatech::soulsifter::Style*> a3;
  for (uint32_t i = 0; i < a3Array.Length(); ++i) {
    if (!a3Array.Get(i).IsObject()) {
      Napi::TypeError::New(env, "TypeError: Object expected (for a3Array.Get(i))").ThrowAsJavaScriptException();
      return env.Null();
    }
    dogatech::soulsifter::Style* x(Napi::ObjectWrap<Style>::Unwrap(a3Array.Get(i).As<Napi::Object>())->getWrappedValue());
    a3.push_back(x);
  }
  if (!info[4].IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for info[4])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Array a4Array = info[4].As<Napi::Array>();
  std::vector<dogatech::soulsifter::Song*> a4;
  for (uint32_t i = 0; i < a4Array.Length(); ++i) {
    if (!a4Array.Get(i).IsObject()) {
      Napi::TypeError::New(env, "TypeError: Object expected (for a4Array.Get(i))").ThrowAsJavaScriptException();
      return env.Null();
    }
    dogatech::soulsifter::Song* x(Napi::ObjectWrap<Song>::Unwrap(a4Array.Get(i).As<Napi::Object>())->getWrappedValue());
    a4.push_back(x);
  }
  if (!info[5].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[5])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a5(info[5].As<Napi::Number>().Int32Value());
  if (!info[6].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[6])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a6(info[6].As<Napi::Number>().Int32Value());
  if (!info[7].IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for info[7])").ThrowAsJavaScriptException();
    return env.Null();
  }
  bool a7(info[7].As<Napi::Boolean>().Value());
  if (!info[8].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[8])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a8(info[8].As<Napi::Number>().Int32Value());
  if (!info[9].IsFunction()) {
    Napi::TypeError::New(env, "TypeError: Function expected (for info[9])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Function a9Fn = info[9].As<Napi::Function>();
  auto a9 = [&env, &a9Fn](string p0) {
    a9Fn.Call(env.Global(), {Napi::String::New(env, p0)});
  };
  std::vector<dogatech::soulsifter::Song*>* result =
      dogatech::soulsifter::SearchUtil::searchSongs(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);

  Napi::Array a = Napi::Array::New(env, static_cast<int>(result->size()));
  for (int i = 0; i < (int) result->size(); i++) {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue((*result)[i], true);
    a.Set(i, instance);
  }
  delete result;
  return a;
}

