#include <iostream>
#include <node.h>
#include <nan.h>
#include "AudioAnalyzer_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Nan::Persistent<v8::Function> AudioAnalyzer::constructor;

AudioAnalyzer::AudioAnalyzer() : Nan::ObjectWrap(), audioanalyzer(NULL), ownWrappedObject(true) {};
AudioAnalyzer::AudioAnalyzer(dogatech::soulsifter::AudioAnalyzer* o) : Nan::ObjectWrap(), audioanalyzer(o), ownWrappedObject(true) {};
AudioAnalyzer::~AudioAnalyzer() { if (ownWrappedObject) delete audioanalyzer; };

void AudioAnalyzer::setNwcpValue(dogatech::soulsifter::AudioAnalyzer* v, bool own) {
  if (ownWrappedObject)
    delete audioanalyzer;
  audioanalyzer = v;
  ownWrappedObject = own;
}

void AudioAnalyzer::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  AudioAnalyzer* obj = new AudioAnalyzer(new dogatech::soulsifter::AudioAnalyzer());
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> AudioAnalyzer::NewInstance() {
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  return Nan::NewInstance(cons).ToLocalChecked();
}

void AudioAnalyzer::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("AudioAnalyzer").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  // Unable to process analyzeKey
  Nan::SetMethod(tpl, "analyzeBpm", analyzeBpm);
  Nan::SetMethod(tpl, "analyzeBpms", analyzeBpms);
  Nan::SetMethod(tpl, "analyzeDuration", analyzeDuration);
  Nan::SetMethod(tpl, "analyzeDurations", analyzeDurations);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  exports->Set(Nan::GetCurrentContext(), Nan::New<v8::String>("AudioAnalyzer").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

void AudioAnalyzer::analyzeBpm(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Song* a0(Nan::ObjectWrap::Unwrap<Song>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  const std::vector<double> result =
      dogatech::soulsifter::AudioAnalyzer::analyzeBpm(a0);

  v8::Local<v8::Array> a = Nan::New<v8::Array>((int) result.size());
  for (int i = 0; i < (int) result.size(); i++) {
    a->Set(Nan::GetCurrentContext(), Nan::New<v8::Number>(i), Nan::New<v8::Number>(result[i]));
  }
  info.GetReturnValue().Set(a);
}

void AudioAnalyzer::analyzeBpms(const Nan::FunctionCallbackInfo<v8::Value>& info) {

      dogatech::soulsifter::AudioAnalyzer::analyzeBpms();
}

void AudioAnalyzer::analyzeDuration(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dogatech::soulsifter::Song* a0(Nan::ObjectWrap::Unwrap<Song>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked())->getNwcpValue());
  int result =
      dogatech::soulsifter::AudioAnalyzer::analyzeDuration(a0);

  info.GetReturnValue().Set(Nan::New<v8::Integer>(result));
}

void AudioAnalyzer::analyzeDurations(const Nan::FunctionCallbackInfo<v8::Value>& info) {

      dogatech::soulsifter::AudioAnalyzer::analyzeDurations();
}

