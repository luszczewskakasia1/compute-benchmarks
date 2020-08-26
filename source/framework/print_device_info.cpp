#include "print_device_info.h"

#include "framework/configuration.h"
#include "framework/error.h"

PrintDeviceInfoFunction printDeviceInfoFunctions[static_cast<int>(Api::COUNT)];

RegisterPrintDeviceInfoFunction::RegisterPrintDeviceInfoFunction(Api api, PrintDeviceInfoFunction function) {
    auto &slot = printDeviceInfoFunctions[static_cast<int>(api)];
    ERROR_IF(slot != nullptr, "printDeviceInfo function registered multiple times");
    slot = function;
}

void printDeviceInfo() {
    for (int apiIndex = static_cast<int>(Api::FIRST); apiIndex <= static_cast<int>(Api::LAST); apiIndex++) {
        const Api api = static_cast<Api>(apiIndex);
        const Api selectedApi = ::configuration.selectedApi;
        if (api != selectedApi && selectedApi != Api::All) {
            continue;
        }

        auto &printDeviceInfo = printDeviceInfoFunctions[static_cast<int>(api)];

        if (printDeviceInfo == nullptr) {
            continue;
        }

        printDeviceInfo();
    }
}
