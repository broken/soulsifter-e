#ifndef SearchUtil_wrap_h
#define SearchUtil_wrap_h

#include <napi.h>
#include "SearchUtil.h"

class SearchUtil : public Napi::ObjectWrap<SearchUtil> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  SearchUtil(const Napi::CallbackInfo& info);
  ~SearchUtil();

  void setWrappedValue(dogatech::soulsifter::SearchUtil* v, bool own);
  dogatech::soulsifter::SearchUtil* getWrappedValue() const { return searchutil; }

 private:
  static Napi::Value searchSongs(const Napi::CallbackInfo& info);

  dogatech::soulsifter::SearchUtil* searchutil;
  bool ownWrappedObject;
};

#endif
