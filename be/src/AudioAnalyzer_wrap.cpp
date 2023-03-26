#include <napi.h>
#include "AudioAnalyzer_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Napi::FunctionReference* AudioAnalyzer::constructor = nullptr;

AudioAnalyzer::~AudioAnalyzer() { if (ownWrappedObject) delete audioanalyzer; };

void AudioAnalyzer::setWrappedValue(dogatech::soulsifter::AudioAnalyzer* v, bool own) {
  if (ownWrappedObject)
    delete audioanalyzer;
  audioanalyzer = v;
  ownWrappedObject = own;
}

Napi::Object AudioAnalyzer::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "AudioAnalyzer", {
    // Unable to process analyzeKey
    StaticMethod<&AudioAnalyzer::analyzeBpm>("analyzeBpm"),
    StaticMethod<&AudioAnalyzer::analyzeBpmAsync>("analyzeBpmAsync"),
    StaticMethod<&AudioAnalyzer::analyzeBpms>("analyzeBpms"),
    StaticMethod<&AudioAnalyzer::analyzeDuration>("analyzeDuration"),
    StaticMethod<&AudioAnalyzer::analyzeDurations>("analyzeDurations"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("AudioAnalyzer", func);
  return exports;
}

Napi::Object AudioAnalyzer::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = AudioAnalyzer::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

AudioAnalyzer::AudioAnalyzer(const Napi::CallbackInfo& info) : Napi::ObjectWrap<AudioAnalyzer>(info), audioanalyzer(nullptr), ownWrappedObject(true) {
  audioanalyzer = new dogatech::soulsifter::AudioAnalyzer();
}

Napi::Value AudioAnalyzer::analyzeBpm(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  std::vector<double> result =
      dogatech::soulsifter::AudioAnalyzer::analyzeBpm(a0);

  Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    a.Set(i, Napi::Number::New(env, result[i]));
  }
  return a;
}

class AnalyzeBpmAsyncWorker : public Napi::AsyncWorker {
 public:
  AnalyzeBpmAsyncWorker(Napi::Env env, std::shared_ptr<Napi::Promise::Deferred> d, dogatech::soulsifter::Song* a0)
      : Napi::AsyncWorker(env), deferred(d), a0x(*a0), a0(&a0x) {
  }

  ~AnalyzeBpmAsyncWorker() { }

  void Execute() {
    std::future<std::vector<double>> result =
        dogatech::soulsifter::AudioAnalyzer::analyzeBpmAsync(a0);
    res = result.get();
  }

  void OnOK() {
    Napi::Env env = Env();
    Napi::HandleScope scope(env);
    Napi::Array wrapped_result = Napi::Array::New(env, static_cast<int>(res.size()));
    for (int i = 0; i < (int) res.size(); i++) {
      wrapped_result.Set(i, Napi::Number::New(env, res[i]));
    }
    deferred->Resolve(wrapped_result);
  }

  void OnError() {
    Napi::Env env = Env();
    Napi::HandleScope scope(env);
    deferred->Reject(Napi::TypeError::New(env, "Failed to process async function analyzeBpmAsync").Value());
  }

 private:
  std::shared_ptr<Napi::Promise::Deferred> deferred;
  std::vector<double> res;
  dogatech::soulsifter::Song a0x;
  dogatech::soulsifter::Song* a0;
};

Napi::Value AudioAnalyzer::analyzeBpmAsync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  std::shared_ptr<Napi::Promise::Deferred> deferred = std::make_shared<Napi::Promise::Deferred>(Napi::Promise::Deferred::New(env));
  if (info.Length() < 1) {
    deferred->Reject(Napi::String::New(env, "Expected at least 1 argument."));
    return deferred->Promise();
  }
  if (!info[0].IsObject()) {
    deferred->Reject(Napi::String::New(env, "TypeError: Object expected (for info[0])"));
    return deferred->Promise();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  AnalyzeBpmAsyncWorker* w = new AnalyzeBpmAsyncWorker(env, deferred, a0);
  w->Queue();
  return deferred->Promise();
}

void AudioAnalyzer::analyzeBpms(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

      dogatech::soulsifter::AudioAnalyzer::analyzeBpms();
}

Napi::Value AudioAnalyzer::analyzeDuration(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  int result =
      dogatech::soulsifter::AudioAnalyzer::analyzeDuration(a0);

  return Napi::Number::New(env, result);
}

void AudioAnalyzer::analyzeDurations(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

      dogatech::soulsifter::AudioAnalyzer::analyzeDurations();
}

