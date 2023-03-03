#ifndef MusicVideo_wrap_h
#define MusicVideo_wrap_h

#include <napi.h>
#include "MusicVideo.h"

class MusicVideo : public Napi::ObjectWrap<MusicVideo> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  MusicVideo(const Napi::CallbackInfo& info);
  ~MusicVideo();

  void setWrappedValue(dogatech::soulsifter::MusicVideo* v, bool own);
  dogatech::soulsifter::MusicVideo* getWrappedValue() const { return musicvideo; }

 private:
  Napi::Value clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getFilePath(const Napi::CallbackInfo& info);
  void setFilePath(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getThumbnailFilePath(const Napi::CallbackInfo& info);
  void setThumbnailFilePath(const Napi::CallbackInfo& info, const Napi::Value &value);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::MusicVideo* musicvideo;
  bool ownWrappedObject;
};

#endif
