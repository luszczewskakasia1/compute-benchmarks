#pragma once

#include "framework/intel_product/get_intel_product.h"
#include "framework/ocl/opencl.h"
#ifdef USE_PCIACCESS
#include <pciaccess/pciaccess.h>

#define PCI_CLASS_PREHISTORIC           0x00
#define PCI_CLASS_DISPLAY               0x03
#define PCI_CLASS_MULTIMEDIA            0x04
#define PCI_SUBCLASS_MULTIMEDIA_VIDEO   0x00
#define PCI_CLASS_PROCESSOR             0x0b
#define PCI_SUBCLASS_PROCESSOR_COPROC   0x40

#define PCIINFOCLASSES(c)                                               \
    ( (((c) & 0x00ff0000) == (PCI_CLASS_PREHISTORIC << 16))             \
      || (((c) & 0x00ff0000) == (PCI_CLASS_DISPLAY << 16))              \
      || ((((c) & 0x00ffff00) == ((PCI_CLASS_MULTIMEDIA << 16) | (PCI_SUBCLASS_MULTIMEDIA_VIDEO << 8)))) \
      || ((((c) & 0x00ffff00) == ((PCI_CLASS_PROCESSOR << 16) | (PCI_SUBCLASS_PROCESSOR_COPROC << 8)))) )
#endif


inline IntelProduct getIntelProduct(cl_device_id device) {
#ifdef USE_PCIACCESS
    int                     status = 0;

    status = pci_system_init();
    if (status != 0) {
       //std::cout <<  "Couldn't initialize PCI system: status err (" << std::to_string(status) << ")" << std::endl;
    }
    else {
       // check explicit slot without enumeration
       struct pci_device       *devPci;
       devPci  = pci_device_find_by_slot(
                        0, // pci domain
                        3, // pci bus
                        0, // gfx
                        0); //pci function 0

       if (devPci == NULL) {
          struct pci_device_iterator *iter = NULL;
          struct pci_slot_match match = {
                 PCI_MATCH_ANY, PCI_MATCH_ANY, PCI_MATCH_ANY, PCI_MATCH_ANY, 0 };

          iter = pci_slot_match_iterator_create(&match);

          if(iter != NULL ) {
             while ((devPci = pci_device_next(iter)) != NULL) {
                if (PCIINFOCLASSES(devPci->device_class)) {
                   //std::cout << "\tVendor " << std::to_string(devPci->vendor_id) << " DevID " << std::to_string(devPci->device_id) << std::endl;
                   //std::cout << "\tBus " << std::to_string(devPci->bus) << " Dev " << std::to_string(devPci->dev) << " Func " << std::to_string(devPci->func)  << std::endl;
                   const auto product = getIntelProduct(static_cast<uint32_t>(devPci->device_id));
                   pci_system_cleanup();
                   return product;
                }
             }
          }
       }
       else {
          if (devPci->vendor_id == 0x8086) {
             //std::cout <<  "INTEL Vendor id " << std::to_string(devPci->vendor_id) << std::endl;
             const auto product = getIntelProduct(static_cast<uint32_t>(devPci->device_id));
             pci_system_cleanup();
             return product;
          }
       }

    }

    pci_system_cleanup();
#endif
    return IntelProduct::Unknown;
}

inline IntelProduct getIntelProduct(const Opencl &opencl) {
    return getIntelProduct(opencl.device);
}

template <typename Arg>
inline IntelGen getIntelGen(Arg &&arg) {
    return getIntelGen(getIntelProduct(std::forward<Arg>(arg)));
}
