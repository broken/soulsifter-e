#ifndef AlbumPart_wrap_h
#define AlbumPart_wrap_h

#include <napi.h>
#include "AlbumPart.h"

class AlbumPart : public Napi::ObjectWrap<AlbumPart> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  AlbumPart(const Napi::CallbackInfo& info);
  ~AlbumPart();

  void setWrappedValue(dogatech::soulsifter::AlbumPart* v, bool own);
  dogatech::soulsifter::AlbumPart* getWrappedValue() const { return albumpart; }

 private:
  Napi::Value clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByPosAndAlbumId(const Napi::CallbackInfo& info);
  static Napi::Value findByPos(const Napi::CallbackInfo& info);
  static Napi::Value findByAlbumId(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getPos(const Napi::CallbackInfo& info);
  void setPos(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getName(const Napi::CallbackInfo& info);
  void setName(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getAlbumId(const Napi::CallbackInfo& info);
  void setAlbumId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getAlbum(const Napi::CallbackInfo& info);
  Napi::Value getAlbumConst(const Napi::CallbackInfo& info);
  void setAlbum(const Napi::CallbackInfo& info, const Napi::Value &value);

  dogatech::soulsifter::AlbumPart* albumpart;
  bool ownWrappedObject;
};

#endif
