#ifndef AlbumPart_wrap_h
#define AlbumPart_wrap_h

#include <node.h>
#include <nan.h>
#include "AlbumPart.h"

class AlbumPart : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::AlbumPart* v, bool own);
  dogatech::soulsifter::AlbumPart* getNwcpValue() const { return albumpart; }

 private:
  AlbumPart();
  explicit AlbumPart(dogatech::soulsifter::AlbumPart* albumpart);
  ~AlbumPart();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void clear(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findById(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByPosAndAlbumId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByPos(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByAlbumId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findAll(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void update(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void sync(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getPos(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setPos(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getName(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setName(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getAlbumId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setAlbumId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getAlbum(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void getAlbumConst(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setAlbum(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::AlbumPart* albumpart;
  bool ownWrappedObject;
};

#endif
