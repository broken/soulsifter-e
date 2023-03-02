#include <napi.h>
#include "TagService_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

TagService::~TagService() { if (ownWrappedObject) delete tagservice; };

void TagService::setWrappedValue(dogatech::soulsifter::TagService* v, bool own) {
  if (ownWrappedObject)
    delete tagservice;
  tagservice = v;
  ownWrappedObject = own;
}

Napi::Object TagService::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "TagService", {
    StaticMethod<&TagService::readId3v2Tag>("readId3v2Tag"),
    StaticMethod<&TagService::writeId3v2Tag>("writeId3v2Tag"),
    StaticMethod<&TagService::updateSongAttributesFromTags>("updateSongAttributesFromTags"),
  });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("TagService", func);
  return exports;
}

Napi::Object TagService::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

TagService::TagService(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TagService>(info), tagservice(nullptr), ownWrappedObject(true) {
  tagservice = new dogatech::soulsifter::TagService();
}

Napi::Value TagService::readId3v2Tag(const Napi::CallbackInfo& info) {
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());

      dogatech::soulsifter::TagService::readId3v2Tag(a0);
}

Napi::Value TagService::writeId3v2Tag(const Napi::CallbackInfo& info) {
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());

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

Napi::Value TagService::updateSongAttributesFromTags(const Napi::CallbackInfo& info) {
  Nan::Callback* a0 = new Nan::Callback();
  a0->Reset(info[0].As<v8::Function>());
  Nan::Callback* cb = new Nan::Callback();
  cb->Reset(info[1].As<v8::Function>());
  Nan::AsyncQueueWorker(new UpdateSongAttributesFromTagsWorker(a0, cb));
}

