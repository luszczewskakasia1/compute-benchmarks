#pragma once

#include "framework/enum/api.h"

using PrintDeviceInfoFunction = void (*)();
struct RegisterSupportedApi {
    RegisterSupportedApi(Api api);
};

bool isApiSupported(Api api);
std::vector<Api> getSupportedApis();