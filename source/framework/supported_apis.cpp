#include "supported_apis.h"

bool supportedApis[static_cast<int>(Api::COUNT)] = {};

RegisterSupportedApi::RegisterSupportedApi(Api api) {
    auto &slot = ::supportedApis[static_cast<int>(api)];
    FATAL_ERROR_IF(slot, "Api support registered multiple times");
    slot = true;
}

bool isApiSupported(Api api) {
    return ::supportedApis[static_cast<int>(api)];
}

std::vector<Api> getSupportedApis() {
    std::vector<Api> apis = {};
    for (int apiIndex = static_cast<int>(Api::FIRST); apiIndex <= static_cast<int>(Api::LAST); apiIndex++) {
        if (::supportedApis[apiIndex]) {
            const Api api = static_cast<Api>(apiIndex);
            apis.push_back(api);
        }
    }
    return apis;
}
