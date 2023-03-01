#ifndef MusicVideoService_wrap_h
#define MusicVideoService_wrap_h

#include <node.h>
#include <nan.h>
#include "MusicVideoService.h"

class MusicVideoService : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::MusicVideoService* v, bool own);
  dogatech::soulsifter::MusicVideoService* getNwcpValue() const { return musicvideoservice; }

 private:
  MusicVideoService();
  explicit MusicVideoService(dogatech::soulsifter::MusicVideoService* musicvideoservice);
  ~MusicVideoService();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void associateYouTubeVideo(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void downloadAudio(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::MusicVideoService* musicvideoservice;
  bool ownWrappedObject;
};

#endif
