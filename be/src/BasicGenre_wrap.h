#ifndef BasicGenre_wrap_h
#define BasicGenre_wrap_h

#include <napi.h>
#include "BasicGenre.h"

class BasicGenre : public Napi::ObjectWrap<BasicGenre> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  BasicGenre(const Napi::CallbackInfo& info);
  ~BasicGenre();

  void setWrappedValue(dogatech::soulsifter::BasicGenre* v, bool own);
  dogatech::soulsifter::BasicGenre* getWrappedValue() const { return basicgenre; }

 private:
  Napi::Value clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByName(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  static Napi::Value findByFilepath(const Napi::CallbackInfo& info);
  static Napi::Value findByArtist(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getName(const Napi::CallbackInfo& info);
  void setName(const Napi::CallbackInfo& info, const Napi::Value &value);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::BasicGenre* basicgenre;
  bool ownWrappedObject;
};

#endif
