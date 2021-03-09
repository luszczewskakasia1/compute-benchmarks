#include "framework/l0/utility/print_device_info_l0.h"
#include "framework/supported_apis.h"

static RegisterPrintDeviceInfoFunction registerFunction(Api::L0, ::printDeviceInfoL0);
static RegisterSupportedApi registerApi(Api::L0);
