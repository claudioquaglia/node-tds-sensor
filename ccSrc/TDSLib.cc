#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "TDSLib.h"
#include "ads1115/ads1115_rpi.h"

using namespace Napi;

Napi::FunctionReference TDSLib::constructor;

Napi::Object TDSLib::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "TDSLib", {
    InstanceMethod("read", &TDSLib::read),
    InstanceMethod("setKValue", &TDSLib::setKValue),
    InstanceMethod("setTemperature", &TDSLib::setTemperature),
    InstanceMethod("setI2CSlave", &TDSLib::setI2CSlave),
    InstanceMethod("getKValue", &TDSLib::getKValue),
    InstanceMethod("getECValue", &TDSLib::getECValue),
    InstanceMethod("getTDSValue", &TDSLib::getTDSValue),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("TDSLib", func);
  return exports;
}

TDSLib::TDSLib(const Napi::CallbackInfo &info) : ObjectWrap(info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Pin number must be an integer value")
        .ThrowAsJavaScriptException();
    return;
  }

  this->_pin = info[0].As<Napi::Number>().ToNumber().Uint32Value();
  this->_kValue = 1; // Set default K-Cell value
  this->_aref = 5.0;
  this->_adcRange = 1024;
  this->_i2cSlave = 0x48;
}

Napi::Value TDSLib::setKValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();;
  }

  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Number expected for K value")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  this->_kValue = info[0].As<Napi::Number>().FloatValue();
  return Napi::Value();
}

Napi::Value TDSLib::setTemperature(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Number expected for tempetature value")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  this->_tempearature = info[0].As<Napi::Number>().FloatValue();

  return Napi::Value();
}

Napi::Value TDSLib::setI2CSlave(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  this->_i2cSlave = info[0].As<Napi::Number>().ToNumber().Uint32Value();

  return Napi::Value();
}

Napi::Value TDSLib::getKValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  return Napi::Number::New(env, this->_kValue);
}

Napi::Value TDSLib::getECValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  return Napi::Number::New(env, this->_ecValue25);
}

Napi::Value TDSLib::getTDSValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  return Napi::Number::New(env, this->_tdsValue);
}

Napi::Value TDSLib::read(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Number expected for tempetature value")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if(openI2CBus("/dev/i2c-1") == -1)
	{
		// return EXIT_FAILURE;
    return env.Undefined();
	}

  auto readValue = readVoltage(info[0].As<Napi::Number>().ToNumber().Uint32Value());
  this->_voltage = readValue/this->_adcRange*this->_aref;
	this->_ecValue=(133.42*this->_voltage*this->_voltage*this->_voltage - 255.86*this->_voltage*this->_voltage + 857.39*this->_voltage)*this->_kValue;
	this->_ecValue25  =  this->_ecValue / (1.0+0.02*(this->_tempearature-25.0));  //temperature compensation
	this->_tdsValue = this->_ecValue25 * TdsFactor;

  return env.Undefined();
}
