#ifndef Album_wrap_h
#define Album_wrap_h

#include <napi.h>
#include "Album.h"

class Album : public Napi::ObjectWrap<Album> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  Album(const Napi::CallbackInfo& info);
  ~Album();

  void setWrappedValue(dogatech::soulsifter::Album* v, bool own);
  dogatech::soulsifter::Album* getWrappedValue() const { return album; }

 private:
  Napi::Value clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByCoverFilepath(const Napi::CallbackInfo& info);
  static Napi::Value findByNameAndArtist(const Napi::CallbackInfo& info);
  static Napi::Value findByName(const Napi::CallbackInfo& info);
  static Napi::Value findByArtist(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  Napi::Value reReleaseDate(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getName(const Napi::CallbackInfo& info);
  void setName(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getArtist(const Napi::CallbackInfo& info);
  void setArtist(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getCoverFilepath(const Napi::CallbackInfo& info);
  void setCoverFilepath(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getMixed(const Napi::CallbackInfo& info);
  void setMixed(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getLabel(const Napi::CallbackInfo& info);
  void setLabel(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getCatalogId(const Napi::CallbackInfo& info);
  void setCatalogId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getReleaseDateYear(const Napi::CallbackInfo& info);
  void setReleaseDateYear(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getReleaseDateMonth(const Napi::CallbackInfo& info);
  void setReleaseDateMonth(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getReleaseDateDay(const Napi::CallbackInfo& info);
  void setReleaseDateDay(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getBasicGenreId(const Napi::CallbackInfo& info);
  void setBasicGenreId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getBasicGenre(const Napi::CallbackInfo& info);
  Napi::Value getBasicGenreConst(const Napi::CallbackInfo& info);
  void setBasicGenre(const Napi::CallbackInfo& info, const Napi::Value &value);

  dogatech::soulsifter::Album* album;
  bool ownWrappedObject;
};

#endif
