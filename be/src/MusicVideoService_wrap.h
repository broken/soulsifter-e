#ifndef MusicVideoService_wrap_h
#define MusicVideoService_wrap_h

#include <napi.h>
#include "MusicVideoService.h"

class MusicVideoService : public Napi::ObjectWrap<MusicVideoService> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  MusicVideoService(const Napi::CallbackInfo& info);
  ~MusicVideoService();

  void setWrappedValue(dogatech::soulsifter::MusicVideoService* v, bool own);
  dogatech::soulsifter::MusicVideoService* getWrappedValue() const { return musicvideoservice; }

 private:
  static Napi::Value associateYouTubeVideo(const Napi::CallbackInfo& info);
  static Napi::Value downloadAudio(const Napi::CallbackInfo& info);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::MusicVideoService* musicvideoservice;
  bool ownWrappedObject;
};

#endif
