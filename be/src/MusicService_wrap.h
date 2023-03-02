#ifndef MusicService_wrap_h
#define MusicService_wrap_h

#include <napi.h>
#include "MusicService.h"

class MusicService : public Napi::ObjectWrap<MusicService> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  MusicService(const Napi::CallbackInfo& info);
  ~MusicService();

  void setWrappedValue(dogatech::soulsifter::MusicService* v, bool own);
  dogatech::soulsifter::MusicService* getWrappedValue() const { return musicservice; }

 private:
  static Napi::Value cleanDirName(const Napi::CallbackInfo& info);

  dogatech::soulsifter::MusicService* musicservice;
  bool ownWrappedObject;
};

#endif
