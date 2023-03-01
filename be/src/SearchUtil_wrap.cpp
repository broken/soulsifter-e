#include <iostream>
#include <node.h>
#include <nan.h>
#include "SearchUtil_wrap.h"
#include "Song.h"
#include "Song_wrap.h"
#include "Style.h"
#include "Style_wrap.h"

Nan::Persistent<v8::Function> SearchUtil::constructor;

SearchUtil::SearchUtil() : Nan::ObjectWrap(), searchutil(NULL), ownWrappedObject(true) {};
SearchUtil::SearchUtil(dogatech::soulsifter::SearchUtil* o) : Nan::ObjectWrap(), searchutil(o), ownWrappedObject(true) {};
SearchUtil::~SearchUtil() { if (ownWrappedObject) delete searchutil; };

void SearchUtil::setNwcpValue(dogatech::soulsifter::SearchUtil* v, bool own) {
  if (ownWrappedObject)
    delete searchutil;
  searchutil = v;
  ownWrappedObject = own;
}

void SearchUtil::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SearchUtil* obj = new SearchUtil(new dogatech::soulsifter::SearchUtil());
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> SearchUtil::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void SearchUtil::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("SearchUtil").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetMethod(tpl, "searchSongs", searchSongs);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("SearchUtil").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void SearchUtil::searchSongs(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  int a1(info[1]->IntegerValue());
  string a2(*v8::String::Utf8Value(info[2]->ToString()));
  v8::Local<v8::Array> a3Array = v8::Local<v8::Array>::Cast(info[3]);
  std::vector<dogatech::soulsifter::Style*> a3;
  for (uint32_t i = 0; i < a3Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a3Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    dogatech::soulsifter::Style* x(Nan::ObjectWrap::Unwrap<Style>(tmp->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
    a3.push_back(x);
  }
  v8::Local<v8::Array> a4Array = v8::Local<v8::Array>::Cast(info[4]);
  std::vector<dogatech::soulsifter::Song*> a4;
  for (uint32_t i = 0; i < a4Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a4Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    dogatech::soulsifter::Song* x(Nan::ObjectWrap::Unwrap<Song>(tmp->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
    a4.push_back(x);
  }
  int a5(info[5]->IntegerValue());
  int a6(info[6]->IntegerValue());
  bool a7(info[7]->BooleanValue());
  int a8(info[8]->IntegerValue());
  Nan::Callback a9Fn;
  a9Fn.Reset(info[9].As<v8::Function>());
  auto a9 = [&a9Fn](string p0) {
    v8::Local<v8::Value> v0 = Nan::New<v8::String>(p0).ToLocalChecked();
    v8::Local<v8::Value> argv[] = {v0};
    a9Fn.Call(1, argv);
  };
  std::vector<dogatech::soulsifter::Song*>* result =
      dogatech::soulsifter::SearchUtil::searchSongs(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result->size());
  for (int i = 0; i < (int) result->size(); i++) {
    v8::Local<v8::Object> instance = Song::NewInstance();
    Song* r = Nan::ObjectWrap::Unwrap<Song>(instance);
    r->setNwcpValue((*result)[i], true);
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete result;
  info.GetReturnValue().Set(a);
}

