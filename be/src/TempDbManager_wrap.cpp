#include <napi.h>
#include "TempDbManager_wrap.h"

Napi::FunctionReference* TempDbManager::constructor = nullptr;

TempDbManager::~TempDbManager() { if (ownWrappedObject) delete tempdbmanager; };

void TempDbManager::setWrappedValue(dogatech::soulsifter::TempDbManager* v, bool own) {
  if (ownWrappedObject)
    delete tempdbmanager;
  tempdbmanager = v;
  ownWrappedObject = own;
}

Napi::Object TempDbManager::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "TempDbManager", {
    // Unable to process getInstance
    InstanceMethod<&TempDbManager::importDirectory>("importDirectory"),
    InstanceMethod<&TempDbManager::ejectTempDb>("ejectTempDb"),
    InstanceMethod<&TempDbManager::ejectAll>("ejectAll"),
    InstanceMethod<&TempDbManager::cleanOrphanedTempDatabases>("cleanOrphanedTempDatabases"),
    InstanceAccessor<&TempDbManager::getActiveSessions>("activeSessions"),
    InstanceMethod<&TempDbManager::hasActiveTempDbs>("hasActiveTempDbs"),
    InstanceAccessor<&TempDbManager::getUnifiedDbName>("unifiedDbName"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("TempDbManager", func);
  return exports;
}

Napi::Object TempDbManager::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = TempDbManager::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

TempDbManager::TempDbManager(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TempDbManager>(info), tempdbmanager(nullptr), ownWrappedObject(false) {
  tempdbmanager = &dogatech::soulsifter::TempDbManager::getInstance();
}

class ImportDirectoryWorker : public Napi::AsyncProgressWorker<float> {
 public:
  ImportDirectoryWorker(std::string a0, Napi::Function& a1)
      : Napi::AsyncProgressWorker<float>(a1) {
    arg0 = a0;
  }

  ~ImportDirectoryWorker() { }

  void Execute(const Napi::AsyncProgressWorker<float>::ExecutionProgress& ep) {
    std::string a0 = arg0;
    auto a1 = [&ep](float p) {
      ep.Send(&p, 1);
    };
    try {
      dogatech::soulsifter::TempDbManager::getInstance().importDirectory(a0, a1);
    } catch (const std::exception& e) {
      SetError(e.what());
    }
  }

  void OnProgress(const float *data, size_t count) {
    Napi::HandleScope scope(Env());
    Callback().Call({Env().Null(), Env().Null(), Napi::Number::New(Env(), *data)});
  }

  void OnOK() {
    Napi::HandleScope scope(Env());
    Callback().Call({Env().Null(), Napi::String::New(Env(), "done"), Env().Null()});
  }

  void OnError(const Napi::Error& e) {
    Napi::HandleScope scope(Env());
    Callback().Call({e.Value(), Env().Null(), Env().Null()});
  }

 private:
  std::string arg0;
};

Napi::Value TempDbManager::importDirectory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  if (!info[1].IsFunction()) {
    Napi::TypeError::New(env, "TypeError: Function expected (for info[1])").ThrowAsJavaScriptException();
    return env.Null();
  }
  Napi::Function a1 = info[1].As<Napi::Function>();
  try {
    ImportDirectoryWorker* w = new ImportDirectoryWorker(a0, a1);
    w->Queue();
    return env.Null();
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value TempDbManager::ejectTempDb(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  TempDbManager* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  try {
    bool result =    obj->tempdbmanager->ejectTempDb(a0);

    return Napi::Boolean::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value TempDbManager::ejectAll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  TempDbManager* obj = this;
  try {
    bool result =    obj->tempdbmanager->ejectAll();

    return Napi::Boolean::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

void TempDbManager::cleanOrphanedTempDatabases(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  TempDbManager* obj = this;
  try {
    obj->tempdbmanager->cleanOrphanedTempDatabases();
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value TempDbManager::getActiveSessions(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  TempDbManager* obj = this;
  try {
    const std::vector<dogatech::soulsifter::TempDbSession>& result =    obj->tempdbmanager->getActiveSessions();

    Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
    for (int i = 0; i < (int) result.size(); i++) {
      Napi::Object instance = Napi::Object::New(env);
      instance.Set("dbName", Napi::String::New(env, result[i].dbName));
      instance.Set("dirPath", Napi::String::New(env, result[i].dirPath));
      instance.Set("styleName", Napi::String::New(env, result[i].styleName));
      instance.Set("styleId", Napi::Number::New(env, result[i].styleId));
      instance.Set("idOffset", Napi::Number::New(env, result[i].idOffset));
      a.Set(i, instance);
    }
    return a;
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value TempDbManager::hasActiveTempDbs(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  TempDbManager* obj = this;
  try {
    bool result =    obj->tempdbmanager->hasActiveTempDbs();

    return Napi::Boolean::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value TempDbManager::getUnifiedDbName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  TempDbManager* obj = this;
  try {
    const std::string& result =    obj->tempdbmanager->getUnifiedDbName();

    return Napi::String::New(env, result);
  } catch (const std::exception& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

