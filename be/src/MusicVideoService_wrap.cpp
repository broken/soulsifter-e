#include <napi.h>
#include "MusicVideoService_wrap.h"
#include "MusicVideo.h"
#include "MusicVideo_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Napi::FunctionReference* MusicVideoService::constructor = nullptr;

MusicVideoService::~MusicVideoService() { if (ownWrappedObject) delete musicvideoservice; };

void MusicVideoService::setWrappedValue(dogatech::soulsifter::MusicVideoService* v, bool own) {
  if (ownWrappedObject)
    delete musicvideoservice;
  musicvideoservice = v;
  ownWrappedObject = own;
}

Napi::Object MusicVideoService::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "MusicVideoService", {
    StaticMethod<&MusicVideoService::associateYouTubeVideo>("associateYouTubeVideo"),
    StaticMethod<&MusicVideoService::downloadAudio>("downloadAudio"),
    StaticMethod<&MusicVideoService::downloadAudioAsync>("downloadAudioAsync"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("MusicVideoService", func);
  return exports;
}

Napi::Object MusicVideoService::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = MusicVideoService::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

MusicVideoService::MusicVideoService(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MusicVideoService>(info), musicvideoservice(nullptr), ownWrappedObject(true) {
  musicvideoservice = new dogatech::soulsifter::MusicVideoService();
}

Napi::Value MusicVideoService::associateYouTubeVideo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 2 arguments.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a1(info[1].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::MusicVideo* result =
      dogatech::soulsifter::MusicVideoService::associateYouTubeVideo(a0, a1);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = MusicVideo::NewInstance(env);
    MusicVideo* r = Napi::ObjectWrap<MusicVideo>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value MusicVideoService::downloadAudio(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  std::vector<string> result =
      dogatech::soulsifter::MusicVideoService::downloadAudio(a0);

  Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    a.Set(i, Napi::String::New(env, result[i]));
  }
  return a;
}

class DownloadAudioAsyncWorker : public Napi::AsyncWorker {
 public:
  DownloadAudioAsyncWorker(Napi::Env env, std::shared_ptr<Napi::Promise::Deferred> d, const std::string& a0)
      : Napi::AsyncWorker(env), deferred(d), a0(a0) {
  }

  ~DownloadAudioAsyncWorker() { }

  void Execute() {
    std::future<std::vector<std::string>> result =
        dogatech::soulsifter::MusicVideoService::downloadAudioAsync(a0);
    res = result.get();
  }

  void OnOK() {
    Napi::Env env = Env();
    Napi::HandleScope scope(env);
    Napi::Array wrapped_result = Napi::Array::New(env, static_cast<int>(res.size()));
    for (int i = 0; i < (int) res.size(); i++) {
      wrapped_result.Set(i, Napi::String::New(env, res[i]));
    }
    deferred->Resolve(wrapped_result);
  }

  void OnError() {
    Napi::Env env = Env();
    Napi::HandleScope scope(env);
    deferred->Reject(Napi::TypeError::New(env, "Failed to process async function downloadAudioAsync").Value());
  }

 private:
  std::shared_ptr<Napi::Promise::Deferred> deferred;
  std::vector<std::string> res;
  const std::string a0;
};

Napi::Value MusicVideoService::downloadAudioAsync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  std::shared_ptr<Napi::Promise::Deferred> deferred = std::make_shared<Napi::Promise::Deferred>(Napi::Promise::Deferred::New(env));
  if (info.Length() < 1) {
    deferred->Reject(Napi::String::New(env, "Expected at least 1 argument."));
    return deferred->Promise();
  }
  if (!info[0].IsString()) {
    deferred->Reject(Napi::String::New(env, "TypeError: String expected (for info[0])"));
    return deferred->Promise();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  DownloadAudioAsyncWorker* w = new DownloadAudioAsyncWorker(env, deferred, a0);
  w->Queue();
  return deferred->Promise();
}

