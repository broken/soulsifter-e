#include <napi.h>
#include "TagService_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Napi::FunctionReference* TagService::constructor = nullptr;

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

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("TagService", func);
  return exports;
}

Napi::Object TagService::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = TagService::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

TagService::TagService(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TagService>(info), tagservice(nullptr), ownWrappedObject(true) {
  tagservice = new dogatech::soulsifter::TagService();
}

void TagService::readId3v2Tag(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());

      dogatech::soulsifter::TagService::readId3v2Tag(a0);
}

void TagService::writeId3v2Tag(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());

      dogatech::soulsifter::TagService::writeId3v2Tag(a0);
}

class UpdateSongAttributesFromTagsWorker : public Napi::AsyncProgressWorker<float> {
 public:
  UpdateSongAttributesFromTagsWorker(Napi::Function& a0)
      : Napi::AsyncProgressWorker<float>(a0) {
  }

  ~UpdateSongAttributesFromTagsWorker() { }

  void Execute(const Napi::AsyncProgressWorker<float>::ExecutionProgress& ep) {
    auto a0 = [&ep](float p) {
      ep.Send(&p, 1);
    };
    dogatech::soulsifter::TagService::updateSongAttributesFromTags(a0);
  }

  void OnProgress(const float *data, size_t count) {
    Napi::HandleScope scope(Env());
    Callback().Call({Env().Null(), Env().Null(), Napi::Number::New(Env(), *data)});
  }

  void OnOK() {
    Napi::HandleScope scope(Env());
    Callback().Call({Env().Null(), Napi::String::New(Env(), "done"), Env().Null()});
  }

 private:
};

void TagService::updateSongAttributesFromTags(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 0 argument.").ThrowAsJavaScriptException();
    return;
  }
  if (!info[0].IsFunction()) {
    Napi::TypeError::New(env, "TypeError: Function expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  Napi::Function a0 = info[0].As<Napi::Function>();
  UpdateSongAttributesFromTagsWorker* w = new UpdateSongAttributesFromTagsWorker(a0);
  w->Queue();
  return;
}

