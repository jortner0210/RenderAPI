#include "ExternalAPI.h"
#include "Internal.h"
#include "Common.h"
#include "Core.h"

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
    _AR_createInstance(v_config);
    return AR_SUCCESS;
}

AR_Result AR_destroyVulkan()
{

}

//////////////////////////////////////////////////////////////////////////////
//////                        AR INTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////

static AR_Result _AR_createInstance(
    ARConfig* v_config
)
{  
    AR_Result res;
    if (!_ar_initialized) {
        // Check Vulkan Validation Layer Support
        res = _AR_checkValidationLayerSupport(v_config->val_layer_cnt, v_config->validation_layers);
        if (AR_ENABLE_VALIDATION_LAYERS && res == AR_VALIDATION_LAYER_NOT_FOUND) {
            AR_EXIT_FAILURE("Requested validation layer not supported!")
        } 

        // Optional for instance creation
		VkApplicationInfo app_info = { 0 };
        app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName   = v_config->app_name;
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName        = "No Engine";
        app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion         = VK_API_VERSION_1_0;

        // Not Optional for instance creation
        VkInstanceCreateInfo create_info;
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;

        // Attach Requested Validation Layers if in debug mode
        if (AR_ENABLE_VALIDATION_LAYERS) {
            create_info.enabledLayerCount   = v_config->val_layer_cnt;
			create_info.ppEnabledLayerNames = "VK_LAYER_LUNARG_standard_validation";//(const char * const *)v_config->validation_layers;
            AR_DEBUG_TRACE_ARG("All Requested Validation Layers Enabled.")
        }
        else
            create_info.enabledLayerCount = 0;

        // Attach Requested Extensions
		//create_info.enabledExtensionCount = 0;  //= init->requested_extension_count;
		create_info.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&create_info.enabledExtensionCount);
		create_info.enabledExtensionCount = 1;
        //for (int i = 0; i < v_config->ext_cnt; i++) 
        //    AR_DEBUG_TRACE_FMT("Extension Requested", v_config->extensions)    
		VkInstance inst;
		AR_DEBUG_TRACE_ARG("All Requested Extensions Enabled.")
		VkResult vk_res = vkCreateInstance(&create_info, NULL, &inst);
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
static AR_Result _AR_checkValidationLayerSupport(
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
    return AR_SUCCESS;
}

