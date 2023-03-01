//
// Handcrafted
//

#include <iostream>
#include <node.h>
#include <nan.h>
#include "SoulSifterSettings_wrap.h"

Nan::Persistent<v8::Function> SoulSifterSettings::constructor;

SoulSifterSettings::SoulSifterSettings() : Nan::ObjectWrap(), soulsiftersettings(NULL), ownWrappedObject(true) {};
SoulSifterSettings::SoulSifterSettings(dogatech::soulsifter::SoulSifterSettings* o) : Nan::ObjectWrap(), soulsiftersettings(o), ownWrappedObject(true) {};
SoulSifterSettings::~SoulSifterSettings() { };

void SoulSifterSettings::setNwcpValue(dogatech::soulsifter::SoulSifterSettings* v, bool own) {
  if (ownWrappedObject)
    delete soulsiftersettings;
  soulsiftersettings = v;
  ownWrappedObject = own;
}

void SoulSifterSettings::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::SoulSifterSettings& ss = dogatech::soulsifter::SoulSifterSettings::getInstance();
  SoulSifterSettings* obj = new SoulSifterSettings(&ss);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> SoulSifterSettings::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void SoulSifterSettings::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("SoulSifterSettings").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "getString", getString);
  Nan::SetPrototypeMethod(tpl, "putString", putString);
  Nan::SetPrototypeMethod(tpl, "getInt", getInt);
  Nan::SetPrototypeMethod(tpl, "putInt", putInt);
  Nan::SetPrototypeMethod(tpl, "getBool", getBool);
  Nan::SetPrototypeMethod(tpl, "putBool", putBool);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("SoulSifterSettings").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void SoulSifterSettings::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SoulSifterSettings* obj = Nan::ObjectWrap::Unwrap<SoulSifterSettings>(info.Holder());
  obj->soulsiftersettings->save();
}

void SoulSifterSettings::getString(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SoulSifterSettings* obj = Nan::ObjectWrap::Unwrap<SoulSifterSettings>(info.Holder());
  const std::string a0(*v8::String::Utf8Value(info[0]->ToString()));
  const std::string result = obj->soulsiftersettings->get<std::string>(a0);

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

void SoulSifterSettings::putString(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SoulSifterSettings* obj = Nan::ObjectWrap::Unwrap<SoulSifterSettings>(info.Holder());
  const std::string a0(*v8::String::Utf8Value(info[0]->ToString()));
  const std::string a1(*v8::String::Utf8Value(info[1]->ToString()));
  obj->soulsiftersettings->put<std::string>(a0, a1);
}

void SoulSifterSettings::getInt(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SoulSifterSettings* obj = Nan::ObjectWrap::Unwrap<SoulSifterSettings>(info.Holder());
  const std::string a0(*v8::String::Utf8Value(info[0]->ToString()));
  const int result = obj->soulsiftersettings->get<int>(a0);

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void SoulSifterSettings::putInt(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SoulSifterSettings* obj = Nan::ObjectWrap::Unwrap<SoulSifterSettings>(info.Holder());
  const std::string a0(*v8::String::Utf8Value(info[0]->ToString()));
  const int a1(info[1]->IntegerValue());
  obj->soulsiftersettings->put<int>(a0, a1);
}

void SoulSifterSettings::getBool(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SoulSifterSettings* obj = Nan::ObjectWrap::Unwrap<SoulSifterSettings>(info.Holder());
  const std::string a0(*v8::String::Utf8Value(info[0]->ToString()));
  const bool result = obj->soulsiftersettings->get<bool>(a0);

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void SoulSifterSettings::putBool(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  SoulSifterSettings* obj = Nan::ObjectWrap::Unwrap<SoulSifterSettings>(info.Holder());
  const std::string a0(*v8::String::Utf8Value(info[0]->ToString()));
  const bool a1(info[1]->BooleanValue());
  obj->soulsiftersettings->put<bool>(a0, a1);
}
