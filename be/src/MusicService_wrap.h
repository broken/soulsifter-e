#ifndef MusicService_wrap_h
#define MusicService_wrap_h

#include <node.h>
#include <nan.h>
#include "MusicService.h"

class MusicService : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::MusicService* v, bool own);
  dogatech::soulsifter::MusicService* getNwcpValue() const { return musicservice; }

 private:
  MusicService();
  explicit MusicService(dogatech::soulsifter::MusicService* musicservice);
  ~MusicService();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void cleanDirName(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::MusicService* musicservice;
  bool ownWrappedObject;
};

#endif
