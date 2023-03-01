#ifndef MusicService_wrap_h
#define MusicService_wrap_h

#include <napi.h>
#include "MusicService.h"

class MusicService : public Napi::ObjectWrap<MusicService> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  MusicService(const Napi::CallbackInfo& info);

  void setWrappedObject(dogatech::soulsifter::MusicService* v, bool own);
  dogatech::soulsifter::MusicService* getWrappedObject() const { return musicservice; }

 private:
  ~MusicService();

  static Napi::Value cleanDirName(const Napi::CallbackInfo& info);

  dogatech::soulsifter::MusicService* musicservice;
  bool ownWrappedObject;
};

#endif
