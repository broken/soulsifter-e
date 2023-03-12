#ifndef Mix_wrap_h
#define Mix_wrap_h

#include <napi.h>
#include "Mix.h"

class Mix : public Napi::ObjectWrap<Mix> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  Mix(const Napi::CallbackInfo& info);
  ~Mix();

  void setWrappedValue(dogatech::soulsifter::Mix* v, bool own);
  dogatech::soulsifter::Mix* getWrappedValue() const { return mix; }

 private:
  void clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByOutSongIdAndInSongId(const Napi::CallbackInfo& info);
  static Napi::Value findByOutSongId(const Napi::CallbackInfo& info);
  static Napi::Value findByInSongId(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  static Napi::Value mixoutCountForRESongId(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getOutSongId(const Napi::CallbackInfo& info);
  void setOutSongId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getOutSong(const Napi::CallbackInfo& info);
  Napi::Value getOutSongConst(const Napi::CallbackInfo& info);
  void setOutSong(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getInSongId(const Napi::CallbackInfo& info);
  void setInSongId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getInSong(const Napi::CallbackInfo& info);
  Napi::Value getInSongConst(const Napi::CallbackInfo& info);
  void setInSong(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getBpmDiff(const Napi::CallbackInfo& info);
  void setBpmDiff(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getRating(const Napi::CallbackInfo& info);
  void setRating(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getComments(const Napi::CallbackInfo& info);
  void setComments(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getAddon(const Napi::CallbackInfo& info);
  void setAddon(const Napi::CallbackInfo& info, const Napi::Value &value);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::Mix* mix;
  bool ownWrappedObject;
};

#endif
