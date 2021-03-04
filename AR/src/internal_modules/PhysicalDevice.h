#ifndef AR_PHYSICAL_DEVICE_H
#define AR_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>

#include "src/Common.h"
#include "src/Core.h"

//////////////////////////////////////////////////////////////////////////////
//////                  AR PHYSICAL DEVICE FUNCTIONS                    //////
//////////////////////////////////////////////////////////////////////////////

static AR_Result _AR_pickPhysicalDevice();

static AR_Result _AR_isSuitableDevice(
    VkPhysicalDevice device
);

#endif