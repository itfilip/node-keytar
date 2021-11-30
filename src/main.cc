#include "napi.h"
#include "async.h"
#include "keytar.h"

namespace {

Napi::Value SetPassword(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Parameter 'username' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string username = info[1].As<Napi::String>();

  if (!info[2].IsString()) {
    Napi::TypeError::New(env, "Parameter 'password' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string password = info[2].As<Napi::String>();

  SetPasswordWorker* worker = new SetPasswordWorker(
    service,
    username,
    password,
    env);
  worker->Queue();
  return worker->Promise();
}

Napi::Value SetPasswordSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Parameter 'username' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string username = info[1].As<Napi::String>();

  if (!info[2].IsString()) {
    Napi::TypeError::New(env, "Parameter 'password' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string password = info[2].As<Napi::String>();

  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::SetPassword(service,
                                                        username,
                                                        password,
                                                        &error);
  if (result == keytar::FAIL_ERROR) {
    Napi::Error::New(env, error.c_str()).
      ThrowAsJavaScriptException();
  }
  return env.Null();
}

Napi::Value GetPassword(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Parameter 'username' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string username = info[1].As<Napi::String>();

  GetPasswordWorker* worker = new GetPasswordWorker(
    service,
    username,
    env);
  worker->Queue();
  return worker->Promise();
}

Napi::Value GetPasswordSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Parameter 'username' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string username = info[1].As<Napi::String>();

  std::string password;
  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::GetPassword(service,
                                                username,
                                                &password,
                                                &error);

  if (result == keytar::FAIL_ERROR) {
    Napi::Error::New(env, error.c_str()).
      ThrowAsJavaScriptException();
    return env.Null();
  } else if (result == keytar::FAIL_NONFATAL) {
    return env.Null();
  } else {
    return Napi::String::New(env, password.data(), password.length());
  }
}

Napi::Value DeletePassword(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Parameter 'username' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string username = info[1].As<Napi::String>();

  DeletePasswordWorker *worker = new DeletePasswordWorker(
    service,
    username,
    env);
  worker->Queue();
  return worker->Promise();
}

Napi::Value DeletePasswordSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Parameter 'username' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string username = info[1].As<Napi::String>();

  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::DeletePassword(service,
    username, &error);
  if (result == keytar::FAIL_ERROR) {
    Napi::Error::New(env, error.c_str()).
      ThrowAsJavaScriptException();
    return env.Null();
  } else if (result == keytar::FAIL_NONFATAL) {
    return Napi::Boolean::New(env, false);
  } else {
    return Napi::Boolean::New(env, true);
  }
}

Napi::Value FindPassword(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  FindPasswordWorker* worker = new FindPasswordWorker(
    service,
    env);
  worker->Queue();
  return worker->Promise();
}

Napi::Value FindPasswordSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  std::string password;
  std::string error;
  keytar::KEYTAR_OP_RESULT result = keytar::FindPassword(service,
                                                 &password,
                                                 &error);
  if (result == keytar::FAIL_ERROR) {
    Napi::Error::New(env, error.c_str()).
      ThrowAsJavaScriptException();
    return env.Null();
  } else if (result == keytar::FAIL_NONFATAL) {
    return env.Null();
  } else {
    return Napi::String::New(env, password.data(), password.length());
  }
}

Napi::Value FindCredentials(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Parameter 'service' must be a string").
      ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string service = info[0].As<Napi::String>();

  FindCredentialsWorker* worker = new FindCredentialsWorker(
    service,
    env);
  worker->Queue();
  return worker->Promise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("getPassword", Napi::Function::New(env, GetPassword));
  exports.Set("getPasswordSync", Napi::Function::New(env, GetPasswordSync));
  exports.Set("setPassword", Napi::Function::New(env, SetPassword));
  exports.Set("setPasswordSync", Napi::Function::New(env, SetPasswordSync));
  exports.Set("deletePassword", Napi::Function::New(env, DeletePassword));
  exports.Set("deletePasswordSync", Napi::Function::New(env, DeletePasswordSync));
  exports.Set("findPassword", Napi::Function::New(env, FindPassword));
  exports.Set("findPasswordSync", Napi::Function::New(env, FindPasswordSync));
  exports.Set("findCredentials", Napi::Function::New(env, FindCredentials));
  return exports;
}

}  // namespace

NODE_API_MODULE(keytar, Init)
