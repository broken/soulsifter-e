#include <napi.h>
#include "SearchUtil_wrap.h"
#include "Playlist.h"
#include "Playlist_wrap.h"
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
  if (info.Length() < 11) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
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
  if (!info[2].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[2])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a2(info[2].As<Napi::Number>().Int32Value());
  if (!info[3].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[3])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a3(info[3].As<Napi::String>().Utf8Value());
  if (!info[4].IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for info[4])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Array a4Array = info[4].As<Napi::Array>();
  std::vector<dogatech::soulsifter::Style*> a4;
  for (uint32_t i = 0; i < a4Array.Length(); ++i) {
    if (!a4Array.Get(i).IsObject()) {
      Napi::TypeError::New(env, "TypeError: Object expected (for a4Array.Get(i))").ThrowAsJavaScriptException();
      return env.Null();
    }
    dogatech::soulsifter::Style* x(Napi::ObjectWrap<Style>::Unwrap(a4Array.Get(i).As<Napi::Object>())->getWrappedValue());
    a4.push_back(x);
  }
  if (!info[5].IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for info[5])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Array a5Array = info[5].As<Napi::Array>();
  std::vector<dogatech::soulsifter::Song*> a5;
  for (uint32_t i = 0; i < a5Array.Length(); ++i) {
    if (!a5Array.Get(i).IsObject()) {
      Napi::TypeError::New(env, "TypeError: Object expected (for a5Array.Get(i))").ThrowAsJavaScriptException();
      return env.Null();
    }
    dogatech::soulsifter::Song* x(Napi::ObjectWrap<Song>::Unwrap(a5Array.Get(i).As<Napi::Object>())->getWrappedValue());
    a5.push_back(x);
  }
  if (!info[6].IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for info[6])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Array a6Array = info[6].As<Napi::Array>();
  std::vector<dogatech::soulsifter::Playlist*> a6;
  for (uint32_t i = 0; i < a6Array.Length(); ++i) {
    if (!a6Array.Get(i).IsObject()) {
      Napi::TypeError::New(env, "TypeError: Object expected (for a6Array.Get(i))").ThrowAsJavaScriptException();
      return env.Null();
    }
    dogatech::soulsifter::Playlist* x(Napi::ObjectWrap<Playlist>::Unwrap(a6Array.Get(i).As<Napi::Object>())->getWrappedValue());
    a6.push_back(x);
  }
  if (!info[7].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[7])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a7(info[7].As<Napi::Number>().Int32Value());
  if (!info[8].IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for info[8])").ThrowAsJavaScriptException();
    return env.Null();
  }
  bool a8(info[8].As<Napi::Boolean>().Value());
  if (!info[9].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[9])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a9(info[9].As<Napi::Number>().Int32Value());
  if (!info[10].IsFunction()) {
    Napi::TypeError::New(env, "TypeError: Function expected (for info[10])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Function a10Fn = info[10].As<Napi::Function>();
  auto a10 = [&env, &a10Fn](string p0) {
    a10Fn.Call(env.Global(), {Napi::String::New(env, p0)});
  };
  std::vector<dogatech::soulsifter::Song*>* result =
      dogatech::soulsifter::SearchUtil::searchSongs(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);

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

