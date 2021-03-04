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

        uint32_t glfw_ext_cnt = 0;
        const char **glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_cnt);

        VkApplicationInfo appInfo  = {0};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext              = NULL;
        appInfo.pApplicationName   = "Vulkan Program Template";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName        = "No Engine";
        appInfo.engineVersion      = 1;
        appInfo.apiVersion         = VK_MAKE_VERSION(1, 0, 0);

        VkInstanceCreateInfo instInfo    = {0};
        instInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instInfo.pNext                   = NULL;
        instInfo.flags                   = 0;
        instInfo.pApplicationInfo        = &appInfo;
        instInfo.enabledExtensionCount   = glfw_ext_cnt;
        instInfo.ppEnabledExtensionNames = glfw_extensions;

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

