#ifndef Playlist_wrap_h
#define Playlist_wrap_h

#include <napi.h>
#include "Playlist.h"

class Playlist : public Napi::ObjectWrap<Playlist> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  Playlist(const Napi::CallbackInfo& info);
  ~Playlist();

  void setWrappedValue(dogatech::soulsifter::Playlist* v, bool own);
  dogatech::soulsifter::Playlist* getWrappedValue() const { return playlist; }

 private:
  void clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByName(const Napi::CallbackInfo& info);
  static Napi::Value findBySpotifyId(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  Napi::Value erase(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getName(const Napi::CallbackInfo& info);
  void setName(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getQuery(const Napi::CallbackInfo& info);
  void setQuery(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getGmusicId(const Napi::CallbackInfo& info);
  void setGmusicId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getYoutubeId(const Napi::CallbackInfo& info);
  void setYoutubeId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getSpotifyId(const Napi::CallbackInfo& info);
  void setSpotifyId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getStyleIds(const Napi::CallbackInfo& info);
  void setStyleIds(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getStyles(const Napi::CallbackInfo& info);
  void setStyles(const Napi::CallbackInfo& info, const Napi::Value &value);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::Playlist* playlist;
  bool ownWrappedObject;
};

#endif
