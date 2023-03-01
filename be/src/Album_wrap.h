#ifndef Album_wrap_h
#define Album_wrap_h

#include <node.h>
#include <nan.h>
#include "Album.h"

class Album : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::Album* v, bool own);
  dogatech::soulsifter::Album* getNwcpValue() const { return album; }

 private:
  Album();
  explicit Album(dogatech::soulsifter::Album* album);
  ~Album();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void clear(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findById(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByCoverFilepath(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByNameAndArtist(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByName(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByArtist(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findAll(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void update(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void sync(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void reReleaseDate(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getName(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setName(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getArtist(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setArtist(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getCoverFilepath(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setCoverFilepath(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getMixed(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setMixed(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getLabel(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setLabel(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getCatalogId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setCatalogId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getReleaseDateYear(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setReleaseDateYear(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getReleaseDateMonth(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setReleaseDateMonth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getReleaseDateDay(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setReleaseDateDay(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getBasicGenreId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setBasicGenreId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getBasicGenre(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void getBasicGenreConst(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setBasicGenre(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::Album* album;
  bool ownWrappedObject;
};

#endif
