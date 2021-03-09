#include "framework/ocl/utility/print_device_info_ocl.h"

static RegisterPrintDeviceInfoFunction registerFunction(Api::OpenCL, printDeviceInfoOcl);
