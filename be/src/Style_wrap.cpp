#include <iostream>
#include <node.h>
#include <nan.h>
#include "Style_wrap.h"
#include "ResultSetIterator.h"
#include "Style.h"
#include "Style_wrap.h"

Nan::Persistent<v8::Function> Style::constructor;

Style::Style() : Nan::ObjectWrap(), style(NULL), ownWrappedObject(true) {};
Style::Style(dogatech::soulsifter::Style* o) : Nan::ObjectWrap(), style(o), ownWrappedObject(true) {};
Style::~Style() { if (ownWrappedObject) delete style; };

void Style::setNwcpValue(dogatech::soulsifter::Style* v, bool own) {
  if (ownWrappedObject)
    delete style;
  style = v;
  ownWrappedObject = own;
}

void Style::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Style* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::Style* xtmp(Nan::ObjectWrap::Unwrap<Style>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::Style& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::Style(x);
  } else {
    wrappedObj = new dogatech::soulsifter::Style();
  }

  Style* obj = new Style(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> Style::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void Style::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Style").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findByREId", findByREId);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetMethod(tpl, "findAllParents", findAllParents);
  // Unable to process findAllSortedByName
  // Unable to process findAllSortedByREId
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("name").ToLocalChecked(), getName, setName);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("rEId").ToLocalChecked(), getREId, setREId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("rELabel").ToLocalChecked(), getRELabel, setRELabel);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("childIds").ToLocalChecked(), getChildIds, setChildIds);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("children").ToLocalChecked(), getChildren, setChildren);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("parentIds").ToLocalChecked(), getParentIds, setParentIds);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("parents").ToLocalChecked(), getParents, setParents);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("Style").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void Style::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  obj->style->clear();
}

void Style::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::Style* result =
      dogatech::soulsifter::Style::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Style::NewInstance();
    Style* r = Nan::ObjectWrap::Unwrap<Style>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Style::findByREId(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::Style* result =
      dogatech::soulsifter::Style::findByREId(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = Style::NewInstance();
    Style* r = Nan::ObjectWrap::Unwrap<Style>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void Style::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::Style>* result =
      dogatech::soulsifter::Style::findAll();

  vector<dogatech::soulsifter::Style*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Style* o = Nan::ObjectWrap::Unwrap<Style>(instance);
    o->style = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void Style::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  int result =  obj->style->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Style::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  int result =  obj->style->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void Style::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  bool result =  obj->style->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void Style::findAllParents(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::Style>* result =
      dogatech::soulsifter::Style::findAllParents();

  vector<dogatech::soulsifter::Style*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    Style* o = Nan::ObjectWrap::Unwrap<Style>(instance);
    o->style = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

NAN_GETTER(Style::getId) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const int result =  obj->style->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Style::setId) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  int a0(value->IntegerValue());
  obj->style->setId(a0);
}

NAN_GETTER(Style::getName) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const string result =  obj->style->getName();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Style::setName) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->style->setName(a0);
}

NAN_GETTER(Style::getREId) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const int result =  obj->style->getREId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(Style::setREId) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  int a0(value->IntegerValue());
  obj->style->setREId(a0);
}

NAN_GETTER(Style::getRELabel) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const string result =  obj->style->getRELabel();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(Style::setRELabel) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->style->setRELabel(a0);
}

NAN_GETTER(Style::getChildIds) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const std::vector<int> result =  obj->style->getChildIds();

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result.size());
  for (int i = 0; i < (int) result.size(); i++) {
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), Nan::New<v8::Integer>(result[i]));
  }
  info.GetReturnValue().Set(a);
}

NAN_SETTER(Style::setChildIds) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  v8::Local<v8::Array> a0Array = v8::Local<v8::Array>::Cast(value);
  std::vector<int> a0;
  for (uint32_t i = 0; i < a0Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a0Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    int x(tmp->IntegerValue());
    a0.push_back(x);
  }
  obj->style->setChildIds(a0);
}

NAN_GETTER(Style::getChildren) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const std::vector<dogatech::soulsifter::Style*> result =  obj->style->getChildren();

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result.size());
  for (int i = 0; i < (int) result.size(); i++) {
    v8::Local<v8::Object> instance = Style::NewInstance();
    Style* r = Nan::ObjectWrap::Unwrap<Style>(instance);
    r->setNwcpValue((result)[i], false);
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  info.GetReturnValue().Set(a);
}

NAN_SETTER(Style::setChildren) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  v8::Local<v8::Array> a0Array = v8::Local<v8::Array>::Cast(value);
  std::vector<dogatech::soulsifter::Style*> a0;
  for (uint32_t i = 0; i < a0Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a0Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    dogatech::soulsifter::Style* x(Nan::ObjectWrap::Unwrap<Style>(tmp->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
    a0.push_back(x);
  }
  obj->style->setChildren(a0);
}

NAN_GETTER(Style::getParentIds) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const std::vector<int> result =  obj->style->getParentIds();

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result.size());
  for (int i = 0; i < (int) result.size(); i++) {
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), Nan::New<v8::Integer>(result[i]));
  }
  info.GetReturnValue().Set(a);
}

NAN_SETTER(Style::setParentIds) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  v8::Local<v8::Array> a0Array = v8::Local<v8::Array>::Cast(value);
  std::vector<int> a0;
  for (uint32_t i = 0; i < a0Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a0Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    int x(tmp->IntegerValue());
    a0.push_back(x);
  }
  obj->style->setParentIds(a0);
}

NAN_GETTER(Style::getParents) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  const std::vector<dogatech::soulsifter::Style*> result =  obj->style->getParents();

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result.size());
  for (int i = 0; i < (int) result.size(); i++) {
    v8::Local<v8::Object> instance = Style::NewInstance();
    Style* r = Nan::ObjectWrap::Unwrap<Style>(instance);
    r->setNwcpValue((result)[i], false);
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  info.GetReturnValue().Set(a);
}

NAN_SETTER(Style::setParents) {
  Style* obj = Nan::ObjectWrap::Unwrap<Style>(info.Holder());
  v8::Local<v8::Array> a0Array = v8::Local<v8::Array>::Cast(value);
  std::vector<dogatech::soulsifter::Style*> a0;
  for (uint32_t i = 0; i < a0Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a0Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    dogatech::soulsifter::Style* x(Nan::ObjectWrap::Unwrap<Style>(tmp->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
    a0.push_back(x);
  }
  obj->style->setParents(a0);
}

