#ifndef PlaylistEntry_wrap_h
#define PlaylistEntry_wrap_h

#include <napi.h>
#include "PlaylistEntry.h"

class PlaylistEntry : public Napi::ObjectWrap<PlaylistEntry> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  PlaylistEntry(const Napi::CallbackInfo& info);
  ~PlaylistEntry();

  void setWrappedValue(dogatech::soulsifter::PlaylistEntry* v, bool own);
  dogatech::soulsifter::PlaylistEntry* getWrappedValue() const { return playlistentry; }

 private:
  void clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByPlaylistIdAndSongId(const Napi::CallbackInfo& info);
  static Napi::Value findByPlaylistId(const Napi::CallbackInfo& info);
  static Napi::Value findBySongId(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  Napi::Value erase(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getPlaylistId(const Napi::CallbackInfo& info);
  void setPlaylistId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getPlaylist(const Napi::CallbackInfo& info);
  Napi::Value getPlaylistConst(const Napi::CallbackInfo& info);
  void setPlaylist(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getSongId(const Napi::CallbackInfo& info);
  void setSongId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getSong(const Napi::CallbackInfo& info);
  Napi::Value getSongConst(const Napi::CallbackInfo& info);
  void setSong(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getPosition(const Napi::CallbackInfo& info);
  void setPosition(const Napi::CallbackInfo& info, const Napi::Value &value);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::PlaylistEntry* playlistentry;
  bool ownWrappedObject;
};

#endif
