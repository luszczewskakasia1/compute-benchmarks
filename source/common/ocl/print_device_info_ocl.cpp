#include "framework/intel_product/ocl/get_intel_product_ocl.h"
#include "framework/ocl/opencl.h"
#include "framework/print_device_info.h"

static void printDeviceInfoOcl() {
    QueueProperties queueProperties = QueueProperties::create().disable();
    Opencl opencl(queueProperties);
    char bufferString[4096];
    cl_uint bufferUint;

    CL_SUCCESS_OR_TERMINATE(clGetPlatformInfo(opencl.platform, CL_PLATFORM_NAME, sizeof(bufferString), bufferString, nullptr));
    std::cout << "OpenCL Platform: " << bufferString << std::endl;

    CL_SUCCESS_OR_TERMINATE(clGetDeviceInfo(opencl.device, CL_DEVICE_NAME, sizeof(bufferString), bufferString, nullptr));
    std::cout << "\tDevice: " << bufferString << std::endl;

    CL_SUCCESS_OR_TERMINATE(clGetDeviceInfo(opencl.device, CL_DRIVER_VERSION, sizeof(bufferString), bufferString, nullptr));
    std::cout << "\t\tdriverVersion: " << bufferString << std::endl;

    CL_SUCCESS_OR_TERMINATE(clGetDeviceInfo(opencl.device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(bufferUint), &bufferUint, nullptr));
    std::cout << "\t\tcomputeUnits:  " << bufferUint << std::endl;

    CL_SUCCESS_OR_TERMINATE(clGetDeviceInfo(opencl.device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(bufferUint), &bufferUint, nullptr));
    std::cout << "\t\tclockFreq:     " << bufferUint << std::endl;

    IntelProduct intelProduct = getIntelProduct(opencl);
    IntelGen intelGen = getIntelGen(intelProduct);
    if (intelProduct != IntelProduct::Unknown) {
        std::cout << "\t\tintelProduct:  " << std::to_string(intelProduct) << " (intelGen: " << std::to_string(intelGen) << ")\n";
    }

    std::cout << std::endl;
}

static RegisterPrintDeviceInfoFunction registerFunction(Api::OpenCL, printDeviceInfoOcl);
