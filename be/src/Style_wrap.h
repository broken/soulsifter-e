#ifndef Style_wrap_h
#define Style_wrap_h

#include <napi.h>
#include "Style.h"

class Style : public Napi::ObjectWrap<Style> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  Style(const Napi::CallbackInfo& info);
  ~Style();

  void setWrappedValue(dogatech::soulsifter::Style* v, bool own);
  dogatech::soulsifter::Style* getWrappedValue() const { return style; }

 private:
  Napi::Value clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByREId(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  static Napi::Value findAllParents(const Napi::CallbackInfo& info);
  // Unable to process findAllSortedByName
  // Unable to process findAllSortedByREId
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getName(const Napi::CallbackInfo& info);
  void setName(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getREId(const Napi::CallbackInfo& info);
  void setREId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getRELabel(const Napi::CallbackInfo& info);
  void setRELabel(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getChildIds(const Napi::CallbackInfo& info);
  void setChildIds(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getChildren(const Napi::CallbackInfo& info);
  void setChildren(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getParentIds(const Napi::CallbackInfo& info);
  void setParentIds(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getParents(const Napi::CallbackInfo& info);
  void setParents(const Napi::CallbackInfo& info, const Napi::Value &value);

  dogatech::soulsifter::Style* style;
  bool ownWrappedObject;
};

#endif
