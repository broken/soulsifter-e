#include <iostream>
#include <node.h>
#include <nan.h>
#include "MusicVideo_wrap.h"
#include "MusicVideo.h"
#include "MusicVideo_wrap.h"
#include "ResultSetIterator.h"

Nan::Persistent<v8::Function> MusicVideo::constructor;

MusicVideo::MusicVideo() : Nan::ObjectWrap(), musicvideo(NULL), ownWrappedObject(true) {};
MusicVideo::MusicVideo(dogatech::soulsifter::MusicVideo* o) : Nan::ObjectWrap(), musicvideo(o), ownWrappedObject(true) {};
MusicVideo::~MusicVideo() { if (ownWrappedObject) delete musicvideo; };

void MusicVideo::setNwcpValue(dogatech::soulsifter::MusicVideo* v, bool own) {
  if (ownWrappedObject)
    delete musicvideo;
  musicvideo = v;
  ownWrappedObject = own;
}

void MusicVideo::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::MusicVideo* wrappedObj = NULL;
  if (info.Length()) {
    dogatech::soulsifter::MusicVideo* xtmp(Nan::ObjectWrap::Unwrap<MusicVideo>(info[0]->ToObject())->getNwcpValue());
    dogatech::soulsifter::MusicVideo& x = *xtmp;
    wrappedObj = new dogatech::soulsifter::MusicVideo(x);
  } else {
    wrappedObj = new dogatech::soulsifter::MusicVideo();
  }

  MusicVideo* obj = new MusicVideo(wrappedObj);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> MusicVideo::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void MusicVideo::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("MusicVideo").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "clear", clear);
  Nan::SetMethod(tpl, "findById", findById);
  Nan::SetMethod(tpl, "findAll", findAll);
  Nan::SetPrototypeMethod(tpl, "update", update);
  Nan::SetPrototypeMethod(tpl, "save", save);
  Nan::SetPrototypeMethod(tpl, "sync", sync);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("id").ToLocalChecked(), getId, setId);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("filePath").ToLocalChecked(), getFilePath, setFilePath);
  Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New<v8::String>("thumbnailFilePath").ToLocalChecked(), getThumbnailFilePath, setThumbnailFilePath);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("MusicVideo").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void MusicVideo::clear(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  obj->musicvideo->clear();
}

void MusicVideo::findById(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int a0(info[0]->IntegerValue());
  dogatech::soulsifter::MusicVideo* result =
      dogatech::soulsifter::MusicVideo::findById(a0);

  if (result == NULL) {
    info.GetReturnValue().SetNull();
  } else {
    v8::Local<v8::Object> instance = MusicVideo::NewInstance();
    MusicVideo* r = Nan::ObjectWrap::Unwrap<MusicVideo>(instance);
    r->setNwcpValue(result, true);

    info.GetReturnValue().Set(instance);
  }
}

void MusicVideo::findAll(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::ResultSetIterator<dogatech::soulsifter::MusicVideo>* result =
      dogatech::soulsifter::MusicVideo::findAll();

  vector<dogatech::soulsifter::MusicVideo*>* v = result->toVector();
  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) v->size());
  for (int i = 0; i < (int) v->size(); i++) {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = Nan::NewInstance(cons).ToLocalChecked();
    MusicVideo* o = Nan::ObjectWrap::Unwrap<MusicVideo>(instance);
    o->musicvideo = (*v)[i];
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), instance);
  }
  delete v;
  info.GetReturnValue().Set(a);
}

void MusicVideo::update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  int result =  obj->musicvideo->update();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void MusicVideo::save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  int result =  obj->musicvideo->save();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void MusicVideo::sync(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  bool result =  obj->musicvideo->sync();

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

NAN_GETTER(MusicVideo::getId) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  const int result =  obj->musicvideo->getId();

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

NAN_SETTER(MusicVideo::setId) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  int a0(value->IntegerValue());
  obj->musicvideo->setId(a0);
}

NAN_GETTER(MusicVideo::getFilePath) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  const string result =  obj->musicvideo->getFilePath();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(MusicVideo::setFilePath) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->musicvideo->setFilePath(a0);
}

NAN_GETTER(MusicVideo::getThumbnailFilePath) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  const string result =  obj->musicvideo->getThumbnailFilePath();

  info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_SETTER(MusicVideo::setThumbnailFilePath) {
  MusicVideo* obj = Nan::ObjectWrap::Unwrap<MusicVideo>(info.Holder());
  string a0(*v8::String::Utf8Value(value->ToString()));
  obj->musicvideo->setThumbnailFilePath(a0);
}

