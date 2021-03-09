#include "framework/ocl/utility/print_device_info_ocl.h"
#include "framework/supported_apis.h"

static RegisterPrintDeviceInfoFunction registerFunction(Api::OpenCL, printDeviceInfoOcl);
static RegisterSupportedApi registerApi(Api::OpenCL);
