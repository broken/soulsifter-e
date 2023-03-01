#ifndef AudioAnalyzer_wrap_h
#define AudioAnalyzer_wrap_h

#include <node.h>
#include <nan.h>
#include "AudioAnalyzer.h"

class AudioAnalyzer : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::AudioAnalyzer* v, bool own);
  dogatech::soulsifter::AudioAnalyzer* getNwcpValue() const { return audioanalyzer; }

 private:
  AudioAnalyzer();
  explicit AudioAnalyzer(dogatech::soulsifter::AudioAnalyzer* audioanalyzer);
  ~AudioAnalyzer();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  // Unable to process analyzeKey
  static void analyzeBpm(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void analyzeBpms(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void analyzeDuration(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void analyzeDurations(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::AudioAnalyzer* audioanalyzer;
  bool ownWrappedObject;
};

#endif
