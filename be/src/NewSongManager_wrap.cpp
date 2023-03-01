#include <iostream>
#include <node.h>
#include <nan.h>
#include "NewSongManager_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Nan::Persistent<v8::Function> NewSongManager::constructor;

NewSongManager::NewSongManager() : Nan::ObjectWrap(), newsongmanager(NULL), ownWrappedObject(true) {};
NewSongManager::NewSongManager(dogatech::soulsifter::NewSongManager* o) : Nan::ObjectWrap(), newsongmanager(o), ownWrappedObject(true) {};
NewSongManager::~NewSongManager() { if (ownWrappedObject) delete newsongmanager; };

void NewSongManager::setNwcpValue(dogatech::soulsifter::NewSongManager* v, bool own) {
  if (ownWrappedObject)
    delete newsongmanager;
  newsongmanager = v;
  ownWrappedObject = own;
}

void NewSongManager::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NewSongManager* obj = new NewSongManager(new dogatech::soulsifter::NewSongManager());
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> NewSongManager::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void NewSongManager::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("NewSongManager").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "import", import);
  Nan::SetPrototypeMethod(tpl, "nextSong", nextSong);
  Nan::SetPrototypeMethod(tpl, "coverImagePath", coverImagePath);
  Nan::SetPrototypeMethod(tpl, "processSong", processSong);
  Nan::SetPrototypeMethod(tpl, "trashMusicFile", trashMusicFile);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("NewSongManager").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void NewSongManager::import(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NewSongManager* obj = Nan::ObjectWrap::Unwrap<NewSongManager>(info.Holder());
  v8::Local<v8::Array> a0Array = v8::Local<v8::Array>::Cast(info[0]);
  std::vector<string> a0;
  for (uint32_t i = 0; i < a0Array->Length(); ++i) {
    v8::Local<v8::Value> tmp = a0Array->Get(Nan::GetCurrentContext(), i).ToLocalChecked();
    string x(*v8::String::Utf8Value(tmp->ToString()));
    a0.push_back(x);
  }
  obj->newsongmanager->import(a0);
}

void NewSongManager::nextSong(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NewSongManager* obj = Nan::ObjectWrap::Unwrap<NewSongManager>(info.Holder());
  dogatech::soulsifter::Song* a0(Nan::ObjectWrap::Unwrap<Song>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  dogatech::soulsifter::Song* a1(Nan::ObjectWrap::Unwrap<Song>(info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  bool result =  obj->newsongmanager->nextSong(a0, a1);

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void NewSongManager::coverImagePath(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NewSongManager* obj = Nan::ObjectWrap::Unwrap<NewSongManager>(info.Holder());
  string result =  obj->newsongmanager->coverImagePath();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

void NewSongManager::processSong(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NewSongManager* obj = Nan::ObjectWrap::Unwrap<NewSongManager>(info.Holder());
  dogatech::soulsifter::Song* a0(Nan::ObjectWrap::Unwrap<Song>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  bool result =  obj->newsongmanager->processSong(a0);

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void NewSongManager::trashMusicFile(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NewSongManager* obj = Nan::ObjectWrap::Unwrap<NewSongManager>(info.Holder());
  dogatech::soulsifter::Song* a0(Nan::ObjectWrap::Unwrap<Song>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  obj->newsongmanager->trashMusicFile(a0);
}

