#include <iostream>
#include <node.h>
#include <nan.h>
#include "MusicService_wrap.h"

Nan::Persistent<v8::Function> MusicService::constructor;

MusicService::MusicService() : Nan::ObjectWrap(), musicservice(NULL), ownWrappedObject(true) {};
MusicService::MusicService(dogatech::soulsifter::MusicService* o) : Nan::ObjectWrap(), musicservice(o), ownWrappedObject(true) {};
MusicService::~MusicService() { if (ownWrappedObject) delete musicservice; };

void MusicService::setNwcpValue(dogatech::soulsifter::MusicService* v, bool own) {
  if (ownWrappedObject)
    delete musicservice;
  musicservice = v;
  ownWrappedObject = own;
}

void MusicService::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MusicService* obj = new MusicService(new dogatech::soulsifter::MusicService());
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> MusicService::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void MusicService::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("MusicService").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetMethod(tpl, "cleanDirName", cleanDirName);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("MusicService").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void MusicService::cleanDirName(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  string a0(*v8::String::Utf8Value(info[0]->ToString()));
  string result =
      dogatech::soulsifter::MusicService::cleanDirName(a0);

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

