#include <napi.h>
#include "Playlist_wrap.h"
#include "Playlist.h"
#include "Playlist_wrap.h"
#include "ResultSetIterator.h"
#include "Style.h"
#include "Style_wrap.h"

Napi::FunctionReference* Playlist::constructor = nullptr;

Playlist::~Playlist() { if (ownWrappedObject) delete playlist; };

void Playlist::setWrappedValue(dogatech::soulsifter::Playlist* v, bool own) {
  if (ownWrappedObject)
    delete playlist;
  playlist = v;
  ownWrappedObject = own;
}

Napi::Object Playlist::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Playlist", {
    InstanceMethod<&Playlist::clear>("clear"),
    StaticMethod<&Playlist::findById>("findById"),
    StaticMethod<&Playlist::findByName>("findByName"),
    StaticMethod<&Playlist::findBySpotifyId>("findBySpotifyId"),
    StaticMethod<&Playlist::findAll>("findAll"),
    InstanceMethod<&Playlist::update>("update"),
    InstanceMethod<&Playlist::save>("save"),
    InstanceMethod<&Playlist::sync>("sync"),
    InstanceMethod<&Playlist::erase>("erase"),
    InstanceAccessor<&Playlist::getId, &Playlist::setId>("id"),
    InstanceAccessor<&Playlist::getName, &Playlist::setName>("name"),
    InstanceAccessor<&Playlist::getQuery, &Playlist::setQuery>("query"),
    InstanceAccessor<&Playlist::getGmusicId, &Playlist::setGmusicId>("gmusicId"),
    InstanceAccessor<&Playlist::getYoutubeId, &Playlist::setYoutubeId>("youtubeId"),
    InstanceAccessor<&Playlist::getSpotifyId, &Playlist::setSpotifyId>("spotifyId"),
    InstanceAccessor<&Playlist::getStyleIds, &Playlist::setStyleIds>("styleIds"),
    InstanceAccessor<&Playlist::getStyles, &Playlist::setStyles>("styles"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("Playlist", func);
  return exports;
}

Napi::Object Playlist::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = Playlist::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

Playlist::Playlist(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Playlist>(info), playlist(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::Playlist* x = Napi::ObjectWrap<Playlist>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue();
    playlist = new dogatech::soulsifter::Playlist(*x);
  } else {
    playlist = new dogatech::soulsifter::Playlist();
  }
}

void Playlist::clear(const Napi::CallbackInfo& info) {
  Playlist* obj = this;
  obj->playlist->clear();
}

Napi::Value Playlist::findById(const Napi::CallbackInfo& info) {
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
  dogatech::soulsifter::Playlist* result =
      dogatech::soulsifter::Playlist::findById(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Playlist::NewInstance(env);
    Playlist* r = Napi::ObjectWrap<Playlist>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Playlist::findByName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Playlist* result =
      dogatech::soulsifter::Playlist::findByName(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Playlist::NewInstance(env);
    Playlist* r = Napi::ObjectWrap<Playlist>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Playlist::findBySpotifyId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Playlist* result =
      dogatech::soulsifter::Playlist::findBySpotifyId(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Playlist::NewInstance(env);
    Playlist* r = Napi::ObjectWrap<Playlist>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Playlist::findAll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  dogatech::ResultSetIterator<dogatech::soulsifter::Playlist>* result =
      dogatech::soulsifter::Playlist::findAll();

  vector<dogatech::soulsifter::Playlist*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Playlist::NewInstance(env);
    Playlist* r = Napi::ObjectWrap<Playlist>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Playlist::update(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  int result =  obj->playlist->update();

  return Napi::Number::New(env, result);
}

Napi::Value Playlist::save(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  int result =  obj->playlist->save();

  return Napi::Number::New(env, result);
}

Napi::Value Playlist::sync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  bool result =  obj->playlist->sync();

  return Napi::Boolean::New(env, result);
}

Napi::Value Playlist::erase(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  int result =  obj->playlist->erase();

  return Napi::Number::New(env, result);
}

Napi::Value Playlist::getId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  int result =  obj->playlist->getId();

  return Napi::Number::New(env, result);
}

void Playlist::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->playlist->setId(a0);
}

Napi::Value Playlist::getName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  const string result =  obj->playlist->getName();

  return Napi::String::New(env, result);
}

void Playlist::setName(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->playlist->setName(a0);
}

Napi::Value Playlist::getQuery(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  const string result =  obj->playlist->getQuery();

  return Napi::String::New(env, result);
}

void Playlist::setQuery(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->playlist->setQuery(a0);
}

Napi::Value Playlist::getGmusicId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  const string result =  obj->playlist->getGmusicId();

  return Napi::String::New(env, result);
}

void Playlist::setGmusicId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->playlist->setGmusicId(a0);
}

Napi::Value Playlist::getYoutubeId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  const string result =  obj->playlist->getYoutubeId();

  return Napi::String::New(env, result);
}

void Playlist::setYoutubeId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->playlist->setYoutubeId(a0);
}

Napi::Value Playlist::getSpotifyId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  const string result =  obj->playlist->getSpotifyId();

  return Napi::String::New(env, result);
}

void Playlist::setSpotifyId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->playlist->setSpotifyId(a0);
}

Napi::Value Playlist::getStyleIds(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  const std::vector<int>& result =  obj->playlist->getStyleIds();

  Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    a.Set(i, Napi::Number::New(env, result[i]));
  }
  return a;
}

void Playlist::setStyleIds(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  Napi::Array a0Array = value.As<Napi::Array>();
  std::vector<int> a0;
  for (uint32_t i = 0; i < a0Array.Length(); ++i) {
    if (!a0Array.Get(i).IsNumber()) {
      Napi::TypeError::New(env, "TypeError: Number expected (for a0Array.Get(i))").ThrowAsJavaScriptException();
      return;
    }
    int32_t x(a0Array.Get(i).As<Napi::Number>().Int32Value());
    a0.push_back(x);
  }
  obj->playlist->setStyleIds(a0);
}

Napi::Value Playlist::getStyles(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Playlist* obj = this;
  const std::vector<dogatech::soulsifter::Style*>& result =  obj->playlist->getStyles();

  Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    Napi::Object instance = Style::NewInstance(env);
    Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
    r->setWrappedValue((result)[i], false);
    a.Set(i, instance);
  }
  return a;
}

void Playlist::setStyles(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Playlist* obj = this;
  if (!value.IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  Napi::Array a0Array = value.As<Napi::Array>();
  std::vector<dogatech::soulsifter::Style*> a0;
  for (uint32_t i = 0; i < a0Array.Length(); ++i) {
    if (!a0Array.Get(i).IsObject()) {
      Napi::TypeError::New(env, "TypeError: Object expected (for a0Array.Get(i))").ThrowAsJavaScriptException();
      return;
    }
    dogatech::soulsifter::Style* x(Napi::ObjectWrap<Style>::Unwrap(a0Array.Get(i).As<Napi::Object>())->getWrappedValue());
    a0.push_back(x);
  }
  obj->playlist->setStyles(a0);
}

