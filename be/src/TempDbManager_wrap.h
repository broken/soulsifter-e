#ifndef TempDbManager_wrap_h
#define TempDbManager_wrap_h

#include <napi.h>
#include "TempDbManager.h"

class TempDbManager : public Napi::ObjectWrap<TempDbManager> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  TempDbManager(const Napi::CallbackInfo& info);
  ~TempDbManager();

  void setWrappedValue(dogatech::soulsifter::TempDbManager* v, bool own);
  dogatech::soulsifter::TempDbManager* getWrappedValue() const { return tempdbmanager; }

 private:
  // Unable to process getInstance
  Napi::Value importDirectory(const Napi::CallbackInfo& info);
  Napi::Value ejectTempDb(const Napi::CallbackInfo& info);
  Napi::Value ejectAll(const Napi::CallbackInfo& info);
  void cleanOrphanedTempDatabases(const Napi::CallbackInfo& info);
  Napi::Value getActiveSessions(const Napi::CallbackInfo& info);
  Napi::Value hasActiveTempDbs(const Napi::CallbackInfo& info);
  void updateDatabaseSchema(const Napi::CallbackInfo& info);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::TempDbManager* tempdbmanager;
  bool ownWrappedObject;
};

#endif
