#pragma once

#include "framework/enum/api.h"

using PrintDeviceInfoFunction = void (*)();
struct RegisterPrintDeviceInfoFunction {
    RegisterPrintDeviceInfoFunction(Api api, PrintDeviceInfoFunction function);
};
extern PrintDeviceInfoFunction printDeviceInfoFunctions[static_cast<int>(Api::COUNT)];

void printDeviceInfo();
