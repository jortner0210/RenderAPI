#ifndef AR_VULKAN_H
#define AR_VULKAN_H

#include <vulkan/vulkan.h>

#include "src/Core.h"

//////////////////////////////////////////////////////////////////////////////
//////                       LVL EXTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////

AR_Result AR_initVulkan(
    ARConfig* v_config
);

AR_Result AR_destroyVulkan();


//////////////////////////////////////////////////////////////////////////////
//////                       LVL INTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////

//
// QUEUE FAMILY STRUCTS
//

#define AR_QUEUE_FAMILY_NOT_SUPPORTED 0x00
#define AR_QUEUE_FAMILY_SUPPORTED     0x01

typedef struct ARCommandProperty
{
    uint32_t q_idx;       // idx of queue family that supports this command
    uint8_t is_supported; // 0 is not supported, 1 is supported
} ARCommandProperty;

typedef struct ARQueueFamilyIndices
{   
    ARCommandProperty graphics_family;
    ARCommandProperty present_family;
} ARQueueFamilyIndices;

//
// VULKAN CONTEXT
//
typedef struct _ARContext 
{
    VkInstance instance;
    VkSurfaceKHR surface;

    // PHYSICAL DEVICE
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceFeatures physical_device_features;

    // LOGICAL DEVICE
    VkDevice device;

    // QUEUES
    VkQueue graphics_queue;
    VkQueue present_queue;

    // QUEUE FAMILY INFO
    ARQueueFamilyIndices q_indices;

} _ARContext;

//
// Global state share between compilation units of LVL
//
extern _ARContext _ar_cxt;
extern uint32_t _ar_initialized;


/////////////////////////////////// AR INSTANCE FUNCTIONS 

static AR_Result AR_createInstance(
    ARConfig* v_config
);

static AR_Result AR_checkValidationLayerSupport(
    uint32_t req_layer_count, 
    char **req_val_layers
);


/////////////////////////////////// AR PHYSICAL DEVICE FUNCTIONS

static AR_Result AR_pickPhysicalDevice();

static AR_Result AR_isSuitableDevice(
    VkPhysicalDevice device
);

/////////////////////////////////// AR QUEUE FAMILY FUNCTIONS

static AR_Result AR_isQueueFamilyComplete(
    ARQueueFamilyIndices *indices
);

static AR_Result AR_findQueueFamilies(
    VkPhysicalDevice device,
    ARQueueFamilyIndices *indices
);

/////////////////////////////////// AR LOGICAL DEVICE FUNCTIONS

static AR_Result AR_createLogicalDevice(
    ARConfig *v_config
);

/////////////////////////////////// AR SURFACE FUNCTIONS

static AR_Result AR_createSurface(
    struct ARConfig* v_config
);

#endif