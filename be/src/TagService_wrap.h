#ifndef TagService_wrap_h
#define TagService_wrap_h

#include <node.h>
#include <nan.h>
#include "TagService.h"

class TagService : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::TagService* v, bool own);
  dogatech::soulsifter::TagService* getNwcpValue() const { return tagservice; }

 private:
  TagService();
  explicit TagService(dogatech::soulsifter::TagService* tagservice);
  ~TagService();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void readId3v2Tag(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void writeId3v2Tag(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void updateSongAttributesFromTags(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::TagService* tagservice;
  bool ownWrappedObject;
};

#endif
