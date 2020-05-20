#pragma once

#include <napi.h>

#define TdsFactor 0.5  // tds = ec / 2

class TDSLib : public Napi::ObjectWrap<TDSLib>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    TDSLib(const Napi::CallbackInfo &);
    Napi::Value read(const Napi::CallbackInfo& info);
    Napi::Value setTemperature(const Napi::CallbackInfo& info);
    Napi::Value setKValue(const Napi::CallbackInfo& info);
    Napi::Value setI2CSlave(const Napi::CallbackInfo& info);
    Napi::Value getKValue(const Napi::CallbackInfo& info);
    Napi::Value getECValue(const Napi::CallbackInfo& info);
    Napi::Value getTDSValue(const Napi::CallbackInfo& info);
    Napi::Value getTemperature(const Napi::CallbackInfo& info);
private:
    static Napi::FunctionReference constructor;
    uint8_t _i2cSlave; /* HEX address */
    int _pin; /* Board pin */
    int _kValue; /* K-Cell constant value of the probe. k = 1 (default) */
    float _aref;  /* default 5.0V */
    float _adcRange; /* 1024 for 10bit ADC;4096 for 12bit ADC */
    float _tempearature; /*Water Temperature*/
    float _voltage; /* Raw ADC To Voltage Conversion*/
    float _ecValue; /* before temperature compensation */
    float _ecValue25; /* after temperature compensation */
    float _tdsValue; /* TDS Value based on 0.5 TDS Factor*/
};
