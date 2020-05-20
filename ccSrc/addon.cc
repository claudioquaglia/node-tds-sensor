#include <napi.h>
#include "TDSLib.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return TDSLib::Init(env, exports);
}

NODE_API_MODULE(addon, Init)
