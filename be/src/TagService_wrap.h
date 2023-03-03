#ifndef TagService_wrap_h
#define TagService_wrap_h

#include <napi.h>
#include "TagService.h"

class TagService : public Napi::ObjectWrap<TagService> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  TagService(const Napi::CallbackInfo& info);
  ~TagService();

  void setWrappedValue(dogatech::soulsifter::TagService* v, bool own);
  dogatech::soulsifter::TagService* getWrappedValue() const { return tagservice; }

 private:
  static Napi::Value readId3v2Tag(const Napi::CallbackInfo& info);
  static Napi::Value writeId3v2Tag(const Napi::CallbackInfo& info);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::TagService* tagservice;
  bool ownWrappedObject;
};

#endif
