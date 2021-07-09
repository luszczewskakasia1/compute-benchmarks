#include "framework/ocl/utility/print_device_info_ocl.inl"
#include "framework/print_device_info.h"
#include "framework/supported_apis.h"
#include "framework/utility/execute_at_app_init.h"

EXECUTE_AT_APP_INIT(RegistrationsOcl) {
    DeviceInfo::registerFunctions(Api::OpenCL, OCL::printDeviceInfo, OCL::printAvailableDevices);
}

static RegisterSupportedApi registerApi(Api::OpenCL);
