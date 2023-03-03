#ifndef NewSongManager_wrap_h
#define NewSongManager_wrap_h

#include <napi.h>
#include "NewSongManager.h"

class NewSongManager : public Napi::ObjectWrap<NewSongManager> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  NewSongManager(const Napi::CallbackInfo& info);
  ~NewSongManager();

  void setWrappedValue(dogatech::soulsifter::NewSongManager* v, bool own);
  dogatech::soulsifter::NewSongManager* getWrappedValue() const { return newsongmanager; }

 private:
  Napi::Value import(const Napi::CallbackInfo& info);
  Napi::Value nextSong(const Napi::CallbackInfo& info);
  Napi::Value coverImagePath(const Napi::CallbackInfo& info);
  Napi::Value processSong(const Napi::CallbackInfo& info);
  Napi::Value trashMusicFile(const Napi::CallbackInfo& info);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::NewSongManager* newsongmanager;
  bool ownWrappedObject;
};

#endif
