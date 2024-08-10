#include <napi.h>
#include "AlertsChannel_wrap.h"

Napi::FunctionReference* AlertsChannel::constructor = nullptr;

AlertsChannel::~AlertsChannel() { if (ownWrappedObject) delete alertschannel; };

void AlertsChannel::setWrappedValue(dogatech::soulsifter::AlertsChannel* v, bool own) {
  if (ownWrappedObject)
    delete alertschannel;
  alertschannel = v;
  ownWrappedObject = own;
}

Napi::Object AlertsChannel::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "AlertsChannel", {
    // Unable to process getInstance
    InstanceMethod<&AlertsChannel::registerChannelEndpoint>("registerChannelEndpoint"),
    InstanceMethod<&AlertsChannel::send>("send"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("AlertsChannel", func);
  return exports;
}

Napi::Object AlertsChannel::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = AlertsChannel::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

AlertsChannel::AlertsChannel(const Napi::CallbackInfo& info) : Napi::ObjectWrap<AlertsChannel>(info), alertschannel(nullptr), ownWrappedObject(false) {
  alertschannel = &dogatech::soulsifter::AlertsChannel::getInstance();
}

void AlertsChannel::registerChannelEndpoint(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  AlertsChannel* obj = this;
  if (!info[0].IsFunction()) {
    Napi::TypeError::New(env, "TypeError: Function expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  Napi::ThreadSafeFunction a0Fn = Napi::ThreadSafeFunction::New(env, info[0].As<Napi::Function>(), "Callback", 0, 1);
  auto a0 = [a0Fn](const std::string& p0) {
    auto callback = [p0](Napi::Env env, Napi::Function jsCallback) {
      jsCallback.Call({Napi::String::New(env, p0)});
    };
    a0Fn.BlockingCall(callback);
  };
  obj->alertschannel->registerChannelEndpoint(a0);
}

void AlertsChannel::send(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  AlertsChannel* obj = this;
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  obj->alertschannel->send(a0);
}

