#ifndef NewSongManager_wrap_h
#define NewSongManager_wrap_h

#include <node.h>
#include <nan.h>
#include "NewSongManager.h"

class NewSongManager : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::NewSongManager* v, bool own);
  dogatech::soulsifter::NewSongManager* getNwcpValue() const { return newsongmanager; }

 private:
  NewSongManager();
  explicit NewSongManager(dogatech::soulsifter::NewSongManager* newsongmanager);
  ~NewSongManager();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void import(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void nextSong(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void coverImagePath(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void processSong(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void trashMusicFile(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::NewSongManager* newsongmanager;
  bool ownWrappedObject;
};

#endif
