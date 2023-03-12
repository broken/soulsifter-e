#ifndef AudioAnalyzer_wrap_h
#define AudioAnalyzer_wrap_h

#include <napi.h>
#include "AudioAnalyzer.h"

class AudioAnalyzer : public Napi::ObjectWrap<AudioAnalyzer> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  AudioAnalyzer(const Napi::CallbackInfo& info);
  ~AudioAnalyzer();

  void setWrappedValue(dogatech::soulsifter::AudioAnalyzer* v, bool own);
  dogatech::soulsifter::AudioAnalyzer* getWrappedValue() const { return audioanalyzer; }

 private:
  // Unable to process analyzeKey
  static Napi::Value analyzeBpm(const Napi::CallbackInfo& info);
  static void analyzeBpms(const Napi::CallbackInfo& info);
  static Napi::Value analyzeDuration(const Napi::CallbackInfo& info);
  static void analyzeDurations(const Napi::CallbackInfo& info);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::AudioAnalyzer* audioanalyzer;
  bool ownWrappedObject;
};

#endif
