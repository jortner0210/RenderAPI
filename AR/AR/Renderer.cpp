#include "Renderer.h"
#include "Common.h"
#include "Core.h"

namespace ar
{

Renderer::Renderer(VulkanConfig &v_config)
{
    VulkanContext::init(v_config);
}



VulkanContext *VulkanContext::m_Instance = nullptr;

void VulkanContext::init(VulkanConfig &v_config)
{

    if (m_Instance == nullptr)
        m_Instance = new VulkanContext(v_config);
    
}

VulkanContext::VulkanContext(VulkanConfig &v_config)
{
    createInstance(v_config);

    createSurface(v_config);

    pickPhysicalDevice();

    createLogicalDevice(v_config);
}

void VulkanContext::createInstance(VulkanConfig &v_config)
{
    bool res;

    res = checkValidationLayerSupport(v_config.validation_layers);
    if (AR_ENABLE_VALIDATION_LAYERS && !res) {
        AR_EXIT_FAILURE("Requested Validation layer not supported!")
    }
    
    // Optional
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext              = nullptr;
    appInfo.pApplicationName   = "Vulkan Program Template";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "No Engine";
    appInfo.engineVersion      = 1;
    appInfo.apiVersion         = VK_MAKE_VERSION(1, 0, 0);

    // Not Optional
    VkInstanceCreateInfo instInfo{};
    instInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext                   = nullptr;
    instInfo.flags                   = 0;
    instInfo.pApplicationInfo        = &appInfo;
    instInfo.enabledExtensionCount   = v_config.extensions.size();
    instInfo.ppEnabledExtensionNames = (const char * const *)v_config.extensions.data();

    if (AR_ENABLE_VALIDATION_LAYERS) {
		instInfo.enabledLayerCount = v_config.validation_layers.size();
        instInfo.ppEnabledLayerNames = (const char * const *)v_config.validation_layers.data();
        AR_DEBUG_TRACE_ARG("All Requested Validation Layers Enabled.")
    }
    else {
        instInfo.enabledLayerCount = 0;
        instInfo.ppEnabledLayerNames = nullptr;
    }

    if (vkCreateInstance(&instInfo, nullptr, &m_VulkanInstance) != VK_SUCCESS) {
        AR_EXIT_FAILURE("Failed to create Vulkan Instance!")
    }

    AR_DEBUG_TRACE_ARG("VULKAN INSTANCE CREATED!")
}

void VulkanContext::createSurface(VulkanConfig &v_config)
{

}

void VulkanContext::pickPhysicalDevice()
{

}

void VulkanContext::createLogicalDevice(VulkanConfig &v_config)
{

}


bool VulkanContext::checkValidationLayerSupport(std::vector<char *> validation_layers)
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    // Check that all the requested layers appear in the available layers
    for (char *layer : validation_layers) {
        bool layer_found = false;

        for (VkLayerProperties &layer_prop : available_layers) {
            if (strcmp(layer, layer_prop.layerName) == 0) {
                AR_DEBUG_TRACE_FMT("VALIDATION LAYER FOUND - %s", layer)
                layer_found = true;
                break;
            }
        }
        if (!layer_found)
            return false;
        
    }
    return true;
}

}