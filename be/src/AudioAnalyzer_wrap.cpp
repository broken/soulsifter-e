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
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(info.Env(), "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  const std::vector<double> result =
      dogatech::soulsifter::AudioAnalyzer::analyzeBpm(a0);

  Napi::Array a = Napi::Array::New(info.Env(), static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    a.Set(i, Napi::Number::New(info.Env(), result[i]));
  }
  return a;
}

Napi::Value AudioAnalyzer::analyzeBpms(const Napi::CallbackInfo& info) {

      dogatech::soulsifter::AudioAnalyzer::analyzeBpms();
}

Napi::Value AudioAnalyzer::analyzeDuration(const Napi::CallbackInfo& info) {
  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Expected at least 1 arguments - received "  + info.Length()).ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  if (!info[0].IsObject()) {
    Napi::TypeError::New(info.Env(), "TypeError: Object expected (for info[0])").ThrowAsJavaScriptException();
    return info.Env().Null();
  }
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());
  int result =
      dogatech::soulsifter::AudioAnalyzer::analyzeDuration(a0);

  return Napi::Number::New(info.Env(), result);
}

Napi::Value AudioAnalyzer::analyzeDurations(const Napi::CallbackInfo& info) {

      dogatech::soulsifter::AudioAnalyzer::analyzeDurations();
}

