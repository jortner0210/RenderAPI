#include <vulkan/vulkan.h>
#include "Instance.h"

//////////////////////////////////////////////////////////////////////////////
//////                        AR INTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////


static AR_Result _AR_createInstance(
    ARConfig* v_config
)
{  
    _AR_checkValidationLayerSupport(v_config->ext_cnt, v_config->extensions);
}

static AR_Result _AR_checkValidationLayerSupport(
    uint32_t req_layer_count, 
    char **req_val_layers
)
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, NULL);
    
    VkLayerProperties *available_layers = (VkLayerProperties *)malloc(sizeof(VkLayerProperties) * layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    for (uint32_t i = 0; i < layer_count; i++) {
        printf("vulkan layers: %s\n", available_layers[i]);
    }
}