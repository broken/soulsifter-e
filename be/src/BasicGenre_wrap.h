#ifndef BasicGenre_wrap_h
#define BasicGenre_wrap_h

#include <node.h>
#include <nan.h>
#include "BasicGenre.h"

class BasicGenre : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::BasicGenre* v, bool own);
  dogatech::soulsifter::BasicGenre* getNwcpValue() const { return basicgenre; }

 private:
  BasicGenre();
  explicit BasicGenre(dogatech::soulsifter::BasicGenre* basicgenre);
  ~BasicGenre();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void clear(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findById(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByName(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findAll(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void update(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void sync(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByFilepath(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByArtist(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getName(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setName(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::BasicGenre* basicgenre;
  bool ownWrappedObject;
};

#endif
