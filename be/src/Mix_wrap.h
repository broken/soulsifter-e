#ifndef Mix_wrap_h
#define Mix_wrap_h

#include <node.h>
#include <nan.h>
#include "Mix.h"

class Mix : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::Mix* v, bool own);
  dogatech::soulsifter::Mix* getNwcpValue() const { return mix; }

 private:
  Mix();
  explicit Mix(dogatech::soulsifter::Mix* mix);
  ~Mix();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void clear(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findById(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByOutSongIdAndInSongId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByOutSongId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByInSongId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findAll(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void update(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void sync(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void mixoutCountForRESongId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getOutSongId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setOutSongId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getOutSong(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void getOutSongConst(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setOutSong(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getInSongId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setInSongId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getInSong(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void getInSongConst(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setInSong(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getBpmDiff(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setBpmDiff(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getRating(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setRating(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getComments(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setComments(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getAddon(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setAddon(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::Mix* mix;
  bool ownWrappedObject;
};

#endif
