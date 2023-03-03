#include <napi.h>
#include "SearchUtil_wrap.h"
#include "Song.h"
#include "Song_wrap.h"
#include "Style.h"
#include "Style_wrap.h"

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

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("SearchUtil", func);
  return exports;
}

Napi::Object SearchUtil::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

SearchUtil::SearchUtil(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SearchUtil>(info), searchutil(nullptr), ownWrappedObject(true) {
  searchutil = new dogatech::soulsifter::SearchUtil();
}

Napi::Value SearchUtil::searchSongs(const Napi::CallbackInfo& info) {
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  int32_t a1(info[1].As<Napi::Number>().Int32Value());
  std::string a2(info[2].As<Napi::String>().Utf8Value());
  Napi::Array a3Array = info[3].As<Napi::Array>();
  std::vector<dogatech::soulsifter::Style*> a3;
  for (uint32_t i = 0; i < a3Array.Length(); ++i) {
    Napi::Value tmp = a3Array.Get(i);
    dogatech::soulsifter::Style* x(Napi::ObjectWrap<Style>::Unwrap(tmp.As<Napi::Object>())->getWrappedValue());
    a3.push_back(x);
  }
  Napi::Array a4Array = info[4].As<Napi::Array>();
  std::vector<dogatech::soulsifter::Song*> a4;
  for (uint32_t i = 0; i < a4Array.Length(); ++i) {
    Napi::Value tmp = a4Array.Get(i);
    dogatech::soulsifter::Song* x(Napi::ObjectWrap<Song>::Unwrap(tmp.As<Napi::Object>())->getWrappedValue());
    a4.push_back(x);
  }
  int32_t a5(info[5].As<Napi::Number>().Int32Value());
  int32_t a6(info[6].As<Napi::Number>().Int32Value());
  bool a7(info[7].As<Napi::Boolean>().Value());
  int32_t a8(info[8].As<Napi::Number>().Int32Value());
  std::vector<dogatech::soulsifter::Song*>* result =
      dogatech::soulsifter::SearchUtil::searchSongs(a0, a1, a2, a3, a4, a5, a6, a7, a8);

  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(result->size()));
  for (int i = 0; i < (int) result->size(); i++) {
    Napi::Object instance = Song::NewInstance(info.Env());
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue((*result)[i], true);
    a.Set(i, instance);
  }
  delete result;
  return a;
}

