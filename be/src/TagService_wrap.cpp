#include <napi.h>
#include "TagService_wrap.h"
#include "Song.h"
#include "Song_wrap.h"

Napi::FunctionReference* TagService::constructor = nullptr;

TagService::~TagService() { if (ownWrappedObject) delete tagservice; };

void TagService::setWrappedValue(dogatech::soulsifter::TagService* v, bool own) {
  if (ownWrappedObject)
    delete tagservice;
  tagservice = v;
  ownWrappedObject = own;
}

Napi::Object TagService::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "TagService", {
    StaticMethod<&TagService::readId3v2Tag>("readId3v2Tag"),
    StaticMethod<&TagService::writeId3v2Tag>("writeId3v2Tag"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("TagService", func);
  return exports;
}

Napi::Object TagService::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = TagService::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

TagService::TagService(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TagService>(info), tagservice(nullptr), ownWrappedObject(true) {
  tagservice = new dogatech::soulsifter::TagService();
}

Napi::Value TagService::readId3v2Tag(const Napi::CallbackInfo& info) {
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());

      dogatech::soulsifter::TagService::readId3v2Tag(a0);
}

Napi::Value TagService::writeId3v2Tag(const Napi::CallbackInfo& info) {
  dogatech::soulsifter::Song* a0(Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue());

      dogatech::soulsifter::TagService::writeId3v2Tag(a0);
}

