#ifndef AR_INTERNAL_H
#define AR_INTERNAL_H

#include <vulkan/vulkan.h>

typedef struct _ARContext 
{
    VkInstance instance;


} _ARContext;

//
// Global state share between compilation units of LVL
//
extern _ARContext _ar_cxt;
extern uint32_t _ar_initialized;

#include "Instance.h"

#endif