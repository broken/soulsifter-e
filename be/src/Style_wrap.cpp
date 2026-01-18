#include <napi.h>
#include "Style_wrap.h"
#include "ResultSetIterator.h"
#include "Style.h"
#include "Style_wrap.h"

Napi::FunctionReference* Style::constructor = nullptr;

Style::~Style() { if (ownWrappedObject) delete style; };

void Style::setWrappedValue(dogatech::soulsifter::Style* v, bool own) {
  if (ownWrappedObject)
    delete style;
  style = v;
  ownWrappedObject = own;
}

Napi::Object Style::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Style", {
    InstanceMethod<&Style::clear>("clear"),
    StaticMethod<&Style::findById>("findById"),
    StaticMethod<&Style::findByREId>("findByREId"),
    StaticMethod<&Style::findAll>("findAll"),
    InstanceMethod<&Style::update>("update"),
    InstanceMethod<&Style::save>("save"),
    InstanceMethod<&Style::sync>("sync"),
    StaticMethod<&Style::findAllParents>("findAllParents"),
    // Unable to process findAllSortedByName
    // Unable to process findAllSortedByREId
    InstanceAccessor<&Style::getId, &Style::setId>("id"),
    InstanceAccessor<&Style::getName, &Style::setName>("name"),
    InstanceAccessor<&Style::getREId, &Style::setREId>("rEId"),
    InstanceAccessor<&Style::getRELabel, &Style::setRELabel>("rELabel"),
    InstanceAccessor<&Style::getDescription, &Style::setDescription>("description"),
    InstanceAccessor<&Style::getChildIds, &Style::setChildIds>("childIds"),
    InstanceAccessor<&Style::getChildren, &Style::setChildren>("children"),
    InstanceAccessor<&Style::getParentIds, &Style::setParentIds>("parentIds"),
    InstanceAccessor<&Style::getParents, &Style::setParents>("parents"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("Style", func);
  return exports;
}

Napi::Object Style::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = Style::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

Style::Style(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Style>(info), style(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::Style* x = Napi::ObjectWrap<Style>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue();
    style = new dogatech::soulsifter::Style(*x);
  } else {
    style = new dogatech::soulsifter::Style();
  }
}

void Style::clear(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    obj->style->clear();
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::findById(const Napi::CallbackInfo& info) {
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
  try {
    dogatech::soulsifter::Style* result =
        dogatech::soulsifter::Style::findById(a0);

    if (result == NULL) {
      return env.Null();
    } else {
      Napi::Object instance = Style::NewInstance(env);
      Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
      r->setWrappedValue(result, true);
      return instance;
    }
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Style::findByREId(const Napi::CallbackInfo& info) {
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
  try {
    dogatech::soulsifter::Style* result =
        dogatech::soulsifter::Style::findByREId(a0);

    if (result == NULL) {
      return env.Null();
    } else {
      Napi::Object instance = Style::NewInstance(env);
      Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
      r->setWrappedValue(result, true);
      return instance;
    }
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Style::findAll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  try {
    dogatech::ResultSetIterator<dogatech::soulsifter::Style>* result =
        dogatech::soulsifter::Style::findAll();

    vector<dogatech::soulsifter::Style*>* v = result->toVector();
    Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
    for (int i = 0; i < (int) v->size(); i++) {
      Napi::Object instance = Style::NewInstance(env);
      Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
      r->setWrappedValue((*v)[i], true);
      a.Set(i, instance);
    }
    delete v;
    return a;
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Style::update(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    int result =    obj->style->update();

    return Napi::Number::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Style::save(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    int result =    obj->style->save();

    return Napi::Number::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Style::sync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    bool result =    obj->style->sync();

    return Napi::Boolean::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Style::findAllParents(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  try {
    dogatech::ResultSetIterator<dogatech::soulsifter::Style>* result =
        dogatech::soulsifter::Style::findAllParents();

    vector<dogatech::soulsifter::Style*>* v = result->toVector();
    Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
    for (int i = 0; i < (int) v->size(); i++) {
      Napi::Object instance = Style::NewInstance(env);
      Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
      r->setWrappedValue((*v)[i], true);
      a.Set(i, instance);
    }
    delete v;
    return a;
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Style::getId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    int result =    obj->style->getId();

    return Napi::Number::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  try {
    obj->style->setId(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    const string& result =    obj->style->getName();

    return Napi::String::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setName(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  try {
    obj->style->setName(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getREId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    int result =    obj->style->getREId();

    return Napi::Number::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setREId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  try {
    obj->style->setREId(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getRELabel(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    const string& result =    obj->style->getRELabel();

    return Napi::String::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setRELabel(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  try {
    obj->style->setRELabel(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getDescription(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    const string& result =    obj->style->getDescription();

    return Napi::String::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setDescription(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  try {
    obj->style->setDescription(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getChildIds(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    const std::vector<int>& result =    obj->style->getChildIds();

    Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
    for (int i = 0; i < (int) result.size(); i++) {
      a.Set(i, Napi::Number::New(env, result[i]));
    }
    return a;
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setChildIds(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
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
  try {
    obj->style->setChildIds(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getChildren(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    const std::vector<dogatech::soulsifter::Style*>& result =    obj->style->getChildren();

    Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
    for (int i = 0; i < (int) result.size(); i++) {
      Napi::Object instance = Style::NewInstance(env);
      Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
      r->setWrappedValue((result)[i], false);
      a.Set(i, instance);
    }
    return a;
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setChildren(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
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
  try {
    obj->style->setChildren(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getParentIds(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    const std::vector<int>& result =    obj->style->getParentIds();

    Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
    for (int i = 0; i < (int) result.size(); i++) {
      a.Set(i, Napi::Number::New(env, result[i]));
    }
    return a;
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setParentIds(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
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
  try {
    obj->style->setParentIds(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Style::getParents(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Style* obj = this;
  try {
    const std::vector<dogatech::soulsifter::Style*>& result =    obj->style->getParents();

    Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
    for (int i = 0; i < (int) result.size(); i++) {
      Napi::Object instance = Style::NewInstance(env);
      Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
      r->setWrappedValue((result)[i], false);
      a.Set(i, instance);
    }
    return a;
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void Style::setParents(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Style* obj = this;
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
  try {
    obj->style->setParents(a0);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

