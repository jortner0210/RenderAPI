#ifndef AR_INTERNAL_H
#define AR_INTERNAL_H

#include <vulkan/vulkan.h>
#include "src/Core.h"

typedef struct _ARContext 
{
    VkInstance instance;


} _ARContext;

//
// Global state share between compilation units of LVL
//
extern _ARContext _ar_cxt;
extern AR_Boolean _ar_initialized;

#include "src/internal_modules/Instance.h"
#include "src/internal_modules/PhysicalDevice.h"

#endif