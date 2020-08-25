#include "framework/l0/levelzero.h"
#include "framework/print_device_info.h"

#include <iomanip>

static void printDeviceInfoL0() {
    LevelZero levelzero;

    ze_driver_properties_t driverProperties{ZE_STRUCTURE_TYPE_DRIVER_PROPERTIES};
    ZE_RESULT_SUCCESS_OR_TERMINATE(zeDriverGetProperties(levelzero.driver, &driverProperties));
    std::cout << "LevelZero driver version: 0x" << std::hex << driverProperties.driverVersion << std::endl;

    ze_device_properties_t deviceProperties{ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
    ZE_RESULT_SUCCESS_OR_TERMINATE(zeDeviceGetProperties(levelzero.device, &deviceProperties));
    std::cout << "\tDevice: " << deviceProperties.name << std::endl;
    std::cout << "\t\tvendorId:     0x" << std::hex << deviceProperties.vendorId << std::endl;
    std::cout << "\t\tdeviceId:     0x" << std::hex << deviceProperties.deviceId << std::endl;
    std::cout << "\t\tclockFreq:    " << std::dec << deviceProperties.coreClockRate << std::endl;
    std::cout << "\t\tconfig:       " << deviceProperties.numSlices << "x" << deviceProperties.numSubslicesPerSlice << "x" << deviceProperties.numEUsPerSubslice << std::endl;
    std::cout << "\t\tthreadsPerEu: " << deviceProperties.numThreadsPerEU << std::endl;

    std::cout << std::endl;
}

static RegisterPrintDeviceInfoFunction registerFunction(Api::L0, printDeviceInfoL0);
