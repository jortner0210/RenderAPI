#include "src/Vulkan.h"
#include "src/Common.h"
#include "src/Core.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

//
// Global state share between compilation units of LVL
//
_ARContext _ar_cxt = { 0 };
uint32_t _ar_initialized = 0;

//////////////////////////////////////////////////////////////////////////////
//////                        AR EXTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////

AR_Result AR_initVulkan(
    ARConfig* v_config
)
{
    AR_createInstance(v_config);

    AR_createSurface(v_config);

    AR_pickPhysicalDevice();

    AR_createLogicalDevice(v_config);

    return AR_SUCCESS;
}

AR_Result AR_destroyVulkan()
{
    // Destroy logical device
    vkDestroyDevice(_ar_cxt.device, NULL);

    // Destroy surface
    vkDestroySurfaceKHR(_ar_cxt.instance, _ar_cxt.surface, NULL);

    // Destroy instance
    vkDestroyInstance(_ar_cxt.instance, NULL);

    _ar_initialized = 0;
    AR_DEBUG_TRACE_ARG("Vulkan instance successfully destroyed!")
}

//////////////////////////////////////////////////////////////////////////////
//////                        AR INTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////                     AR INSTANCE FUNCTIONS                        //////
//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// AR INSTANCE FUNCTIONS 

static AR_Result AR_createInstance(
    ARConfig* v_config
)
{  
    AR_Result res;
    if (!_ar_initialized) {
        // Check Vulkan Validation Layer Support
        res = AR_checkValidationLayerSupport(v_config->val_layer_cnt, v_config->validation_layers);
        if (AR_ENABLE_VALIDATION_LAYERS && res == AR_VALIDATION_LAYER_NOT_FOUND) {
            AR_EXIT_FAILURE("Requested validation layer not supported!")
        } 

        // Optional
        VkApplicationInfo appInfo  = {0};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext              = NULL;
        appInfo.pApplicationName   = "Vulkan Program Template";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = "No Engine";
        appInfo.engineVersion      = 1;
        appInfo.apiVersion         = VK_MAKE_VERSION(1, 0, 0);

        // Not Optional
        VkInstanceCreateInfo instInfo    = {0};
        instInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instInfo.pNext                   = NULL;
        instInfo.flags                   = 0;
        instInfo.pApplicationInfo        = &appInfo;
        instInfo.enabledExtensionCount   = v_config->ext_cnt;
        instInfo.ppEnabledExtensionNames = (const char * const *)v_config->extensions;

        if (AR_ENABLE_VALIDATION_LAYERS) {
			instInfo.enabledLayerCount = v_config->val_layer_cnt;
            instInfo.ppEnabledLayerNames = (const char * const *)v_config->validation_layers;
            AR_DEBUG_TRACE_ARG("All Requested Validation Layers Enabled.")
        }
        else {
            instInfo.enabledLayerCount = 0;
            instInfo.ppEnabledLayerNames = NULL;
        }

        // Create the Vulkan instance.
        VkResult vk_res = vkCreateInstance(&instInfo, NULL, &_ar_cxt.instance);

        if (vk_res != VK_SUCCESS) {
            AR_EXIT_FAILURE("Failed to create Vulkan Instance!")
        }

        _ar_initialized = 1;
    }
    else 
        AR_EXIT_FAILURE("Vulkan Instance already initialized!")
    AR_DEBUG_TRACE_ARG("VULKAN INSTANCE CREATED!")
    return AR_SUCCESS;
}

//
// Check if requested Vulkan Validation Layers are available.
//
static AR_Result AR_checkValidationLayerSupport(
    uint32_t req_layer_count, 
    char **req_val_layers
)
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, NULL);
    
    VkLayerProperties *available_layers = (VkLayerProperties *)malloc(sizeof(VkLayerProperties) * layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    for (uint32_t i = 0; i < req_layer_count; i++) {
        AR_Boolean layer_found = AR_FALSE;

        for (int j = 0; j < layer_count; j++) {
            if (strcmp(req_val_layers[i], available_layers[j].layerName) == 0) {
                AR_DEBUG_TRACE_FMT("VALIDATION LAYER FOUND::%s", req_val_layers[i])
                layer_found = AR_TRUE;
                break;
            }
        }
        if (layer_found == AR_FALSE) {
            return AR_VALIDATION_LAYER_NOT_FOUND;
        }
    }
    free(available_layers);
    return AR_SUCCESS;
}

/////////////////////////////////// AR PHYSICAL DEVICE FUNCTIONS

static AR_Result AR_pickPhysicalDevice()
{
    _ar_cxt.physical_device = VK_NULL_HANDLE;

    // List graphics cards
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(_ar_cxt.instance, &device_count, NULL);
    if (device_count == 0) {
        AR_EXIT_FAILURE("Failed to find GPUs with Vulkan support!");
    }
    
    VkPhysicalDevice *devices = (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * device_count);
    vkEnumeratePhysicalDevices(_ar_cxt.instance, &device_count, devices);

    // Iterate through devices to find one that is suitable
    for (int i = 0; i < device_count; i++) {
        if (AR_isSuitableDevice(devices[i]) == AR_SUCCESS) {
            AR_DEBUG_TRACE_FMT("Suitable device found. Device Number:%d", i);
            _ar_cxt.physical_device = devices[i];
            break;
        }
    }

    // Exit failure if no device found
    if (_ar_cxt.physical_device == VK_NULL_HANDLE) {
        AR_EXIT_FAILURE("Failed to find a suitable device!");
    }

    return AR_SUCCESS;
}

static AR_Result AR_isSuitableDevice(
    VkPhysicalDevice device
)
{
    // Properties queried: name, type, vulkan supported
    VkPhysicalDeviceProperties device_props;
    vkGetPhysicalDeviceProperties(device, &device_props);

    AR_DEBUG_TRACE_FMT("DEVICE NAME: %s", device_props.deviceName);

    // Properties Queried: texture compression, 64 bit floats, multi viewport rendering
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    //
    // IMPLEMENTAION IDEA:
    //  - Score each device based on available props/features
    //  - Pick highest scored device 
    //
 
    AR_findQueueFamilies(device, &_ar_cxt.q_indices);

    return AR_isQueueFamilyComplete(&_ar_cxt.q_indices);
}

/////////////////////////////////// AR QUEUE FAMILY FUNCTIONS

static AR_Result AR_isQueueFamilyComplete(
    ARQueueFamilyIndices *indices
)
{
    if (indices->graphics_family.is_supported == AR_QUEUE_FAMILY_SUPPORTED
        && indices->present_family.is_supported == AR_QUEUE_FAMILY_SUPPORTED) {
        return AR_SUCCESS;
    }
    else {
        return AR_ERROR;
    }
}

static AR_Result AR_findQueueFamilies(
    VkPhysicalDevice device,
    ARQueueFamilyIndices *indices
)
{
    // This function determines the queue families supported by a device
    uint32_t q_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &q_family_count, NULL);

    VkQueueFamilyProperties *q_families = (VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * q_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &q_family_count, q_families);
    int j = 0;
    for (int i = 0; i < q_family_count; i++) {
        // Is the desired command supported by queue?
        // If so link that command to queue using 'i', and set is_supported to true

        // Check for Graphics support
        if (q_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices->graphics_family.q_idx = j;
            indices->graphics_family.is_supported = AR_QUEUE_FAMILY_SUPPORTED;
            AR_DEBUG_TRACE_ARG("Queue Graphics - SUPPORTED!")
            j++;
        }

        // Check for Present Surface Support
        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _ar_cxt.surface, &present_support);
        if (present_support == VK_TRUE) {   
            indices->present_family.q_idx = j;
            indices->present_family.is_supported = AR_QUEUE_FAMILY_SUPPORTED;
            AR_DEBUG_TRACE_ARG("Queue Surface Present - SUPPORTED!")
            j++;
        }

        if (AR_isQueueFamilyComplete(indices)) 
            break;
        
    }
    return AR_SUCCESS;
}

/////////////////////////////////// AR LOGICAL DEVICE FUNCTIONS

static AR_Result AR_createLogicalDevice(
    ARConfig *v_config
)
{
    ARQueueFamilyIndices q_indices;
    AR_findQueueFamilies(_ar_cxt.physical_device, &q_indices);

    // NOTE - Really only need one queue for queue family, can create command buffers
    //        on multiple threads and submit to main thread in single call

    // CREATE LOGICAL DEVICE
    VkDeviceCreateInfo device_create_info = {0};
    device_create_info.sType              = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pEnabledFeatures   = &_ar_cxt.physical_device_features; // TO DO - Specify the set of desired device features

    // Describes the number of queues we want for a single queue family
    float q_priority = 1.0f;
    uint32_t unique_queue_families_count = 2;
    uint32_t *unique_queue_families = (uint32_t *)malloc(sizeof(uint32_t) * unique_queue_families_count);
    
    // TO DO: FIGURE OUT THIS MESS
    unique_queue_families[0] = _ar_cxt.q_indices.graphics_family.q_idx;
    unique_queue_families[1] = _ar_cxt.q_indices.present_family.q_idx;
    VkDeviceQueueCreateInfo *device_q_create_infos = (VkDeviceQueueCreateInfo *)malloc(sizeof(VkDeviceQueueCreateInfo) * unique_queue_families_count);
    memset(device_q_create_infos, 0, sizeof(VkDeviceQueueCreateInfo) * unique_queue_families_count);
    
    for (int i = 0; i < unique_queue_families_count; i++) {
        VkDeviceQueueCreateInfo *q_create_info = &device_q_create_infos[i];
        q_create_info->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        q_create_info->queueFamilyIndex = unique_queue_families[i];
        q_create_info->queueCount = 1;
        q_create_info->pQueuePriorities = &q_priority;
    }

    device_create_info.pQueueCreateInfos = device_q_create_infos;
    device_create_info.queueCreateInfoCount = unique_queue_families_count;

    //
    // NOTE - This is technically unnecessary
    //      - Implementations of Vulkan no longer make a distinction between
    //        instance and device specific validation layers
    //      - This ensures compatibility with older implementations
    //
    device_create_info.enabledExtensionCount = 0;
    if (AR_ENABLE_VALIDATION_LAYERS) {
        device_create_info.enabledLayerCount = v_config->val_layer_cnt;
        device_create_info.ppEnabledLayerNames = (const char * const *)v_config->validation_layers;
    }
    else {
        device_create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(_ar_cxt.physical_device, &device_create_info, NULL, &_ar_cxt.device) != VK_SUCCESS) {
        AR_EXIT_FAILURE("FAILED TO CREATE DEVICE")
    }

    //
    // FILL GRAPHICS QUEUE
    // TO DO - Right now, only one value to set
    //       - This will depend on user specifications
    //
    vkGetDeviceQueue(_ar_cxt.device, _ar_cxt.q_indices.graphics_family.q_idx, 0, &_ar_cxt.graphics_queue);
    vkGetDeviceQueue(_ar_cxt.device, _ar_cxt.q_indices.present_family.q_idx, 0, &_ar_cxt.present_queue);
    AR_DEBUG_TRACE_ARG("Graphics Queue Initialized!")
}

// TO DO - Abstraction for surface?
static AR_Result AR_createSurface(
    struct ARConfig* v_config
)
{
#ifdef __linux__
    if (glfwCreateWindowSurface(_ar_cxt.instance, v_config->window, NULL, &_ar_cxt.surface) != VK_SUCCESS) {
        AR_EXIT_FAILURE("Failed to create window surface!")
    }
    AR_DEBUG_TRACE_ARG("SURFACE REGISTERED")
#elif _WIN32
    VkWin32SurfaceCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.hwnd = glfwGetWin32Window(v_config->window);
    create_info.hinstance = GetModuleHandle(NULL);
    if (vkCreateWin32SurfaceKHR(_ar_cxt.instance, &create_info, NULL, &_ar_cxt.surface) != VK_SUCCESS) {
        AR_EXIT_FAILURE("Failed to create window surface!")
    }
    AR_DEBUG_TRACE_ARG("SURFACE REGISTERED")
#endif
}