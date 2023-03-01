#ifndef SearchUtil_wrap_h
#define SearchUtil_wrap_h

#include <node.h>
#include <nan.h>
#include "SearchUtil.h"

class SearchUtil : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::SearchUtil* v, bool own);
  dogatech::soulsifter::SearchUtil* getNwcpValue() const { return searchutil; }

 private:
  SearchUtil();
  explicit SearchUtil(dogatech::soulsifter::SearchUtil* searchutil);
  ~SearchUtil();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void searchSongs(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::SearchUtil* searchutil;
  bool ownWrappedObject;
};

#endif
