#include "print_device_info_l0.h"

#include "framework/l0/intel_product/get_intel_product_l0.h"
#include "framework/l0/levelzero.h"

#include <iomanip>

void printDeviceInfoL0() {
    ContextProperties contextProperties = ContextProperties::create().disable();
    QueueProperties queueProperties = QueueProperties::create().disable();
    LevelZero levelzero(queueProperties, contextProperties);

    ze_driver_properties_t driverProperties{ZE_STRUCTURE_TYPE_DRIVER_PROPERTIES};
    ZE_RESULT_SUCCESS_OR_ERROR(zeDriverGetProperties(levelzero.driver, &driverProperties));
    std::cout << "LevelZero driver version: 0x" << std::hex << driverProperties.driverVersion << std::endl;

    ze_device_properties_t deviceProperties{ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
    ZE_RESULT_SUCCESS_OR_ERROR(zeDeviceGetProperties(levelzero.device, &deviceProperties));
    IntelProduct intelProduct = getIntelProduct(deviceProperties);
    IntelGen intelGen = getIntelGen(intelProduct);
    std::cout << "\tDevice: " << deviceProperties.name << std::endl;
    std::cout << "\t\tvendorId:     0x" << std::hex << deviceProperties.vendorId << std::endl;
    std::cout << "\t\tdeviceId:     0x" << std::hex << deviceProperties.deviceId << " (intelProduct=" << std::to_string(intelProduct) << ", intelGen=" << std::to_string(intelGen) << ")\n";
    std::cout << "\t\tclockFreq:    " << std::dec << deviceProperties.coreClockRate << std::endl;
    std::cout << "\t\tconfig:       " << deviceProperties.numSlices << "x" << deviceProperties.numSubslicesPerSlice << "x" << deviceProperties.numEUsPerSubslice << std::endl;
    std::cout << "\t\teuCount:      " << deviceProperties.numSlices * deviceProperties.numSubslicesPerSlice * deviceProperties.numEUsPerSubslice << std::endl;
    std::cout << "\t\tthreadsPerEu: " << deviceProperties.numThreadsPerEU << std::endl;

    std::cout << std::endl;
}
