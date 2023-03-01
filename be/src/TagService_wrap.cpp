#include <iostream>
#include <node.h>
#include <nan.h>
#include "TagService_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Nan::Persistent<v8::Function> TagService::constructor;

TagService::TagService() : Nan::ObjectWrap(), tagservice(NULL), ownWrappedObject(true) {};
TagService::TagService(dogatech::soulsifter::TagService* o) : Nan::ObjectWrap(), tagservice(o), ownWrappedObject(true) {};
TagService::~TagService() { if (ownWrappedObject) delete tagservice; };

void TagService::setNwcpValue(dogatech::soulsifter::TagService* v, bool own) {
  if (ownWrappedObject)
    delete tagservice;
  tagservice = v;
  ownWrappedObject = own;
}

void TagService::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  TagService* obj = new TagService(new dogatech::soulsifter::TagService());
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> TagService::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void TagService::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("TagService").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetMethod(tpl, "readId3v2Tag", readId3v2Tag);
  Nan::SetMethod(tpl, "writeId3v2Tag", writeId3v2Tag);
  Nan::SetMethod(tpl, "updateSongAttributesFromTags", updateSongAttributesFromTags);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("TagService").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void TagService::readId3v2Tag(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Song* a0(Nan::ObjectWrap::Unwrap<Song>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());

      dogatech::soulsifter::TagService::readId3v2Tag(a0);
}

void TagService::writeId3v2Tag(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Song* a0(Nan::ObjectWrap::Unwrap<Song>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());

      dogatech::soulsifter::TagService::writeId3v2Tag(a0);
}

class UpdateSongAttributesFromTagsWorker : public Nan::AsyncProgressWorkerBase<float> {
 public:
  UpdateSongAttributesFromTagsWorker(Nan::Callback* a0, Nan::Callback* callback)
      : AsyncProgressWorkerBase(callback), progressCallback(a0) {
  }

  ~UpdateSongAttributesFromTagsWorker() {
    delete progressCallback;
  }

  void Execute(const Nan::AsyncProgressWorkerBase<float>::ExecutionProgress& ep) {
    auto a0 = [&ep](float p) {
      ep.Send(&p, 1);
    };
    dogatech::soulsifter::TagService::updateSongAttributesFromTags(a0);
  }

  void HandleProgressCallback(const float *data, size_t count) {
    Nan::HandleScope scope;
    v8::Local<v8::Value> v = Nan::New<v8::Number>(*data);
    v8::Local<v8::Value> argv[] = {v};
    progressCallback->Call(1, argv);
  }

 private:
  Nan::Callback* progressCallback;
};

void TagService::updateSongAttributesFromTags(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::Callback* a0 = new Nan::Callback();
  a0->Reset(info[0].As<v8::Function>());
  Nan::Callback* cb = new Nan::Callback();
  cb->Reset(info[1].As<v8::Function>());
  Nan::AsyncQueueWorker(new UpdateSongAttributesFromTagsWorker(a0, cb));
}

