#include "print_device_info_ocl.h"

#include "framework/ocl/intel_product/get_intel_product_ocl.h"
#include "framework/ocl/opencl.h"

void printDeviceInfoOcl() {
    ContextProperties contextProperties = ContextProperties::create().disable();
    QueueProperties queueProperties = QueueProperties::create().disable();
    Opencl opencl(queueProperties, contextProperties);
    char bufferString[4096];
    cl_uint bufferUint;

    CL_SUCCESS_OR_ERROR(clGetPlatformInfo(opencl.platform, CL_PLATFORM_NAME, sizeof(bufferString), bufferString, nullptr), "clGetPlatformInfo failed");
    std::cout << "OpenCL Platform: " << bufferString << std::endl;

    CL_SUCCESS_OR_ERROR(clGetDeviceInfo(opencl.device, CL_DEVICE_NAME, sizeof(bufferString), bufferString, nullptr), "clGetDeviceInfo failed");
    std::cout << "\tDevice: " << bufferString << std::endl;

    CL_SUCCESS_OR_ERROR(clGetDeviceInfo(opencl.device, CL_DRIVER_VERSION, sizeof(bufferString), bufferString, nullptr), "clGetDeviceInfo failed");
    std::cout << "\t\tdriverVersion: " << bufferString << std::endl;

    CL_SUCCESS_OR_ERROR(clGetDeviceInfo(opencl.device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(bufferUint), &bufferUint, nullptr), "clGetDeviceInfo failed");
    std::cout << "\t\tcomputeUnits:  " << bufferUint << std::endl;

    CL_SUCCESS_OR_ERROR(clGetDeviceInfo(opencl.device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(bufferUint), &bufferUint, nullptr), "clGetDeviceInfo failed");
    std::cout << "\t\tclockFreq:     " << bufferUint << std::endl;

    IntelProduct intelProduct = getIntelProduct(opencl);
    IntelGen intelGen = getIntelGen(intelProduct);
    if (intelProduct != IntelProduct::Unknown) {
        std::cout << "\t\tintelProduct:  " << std::to_string(intelProduct) << " (intelGen: " << std::to_string(intelGen) << ")\n";
    }

    std::cout << std::endl;
}
