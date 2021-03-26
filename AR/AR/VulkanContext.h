#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Utils.h"

// Vulkan Boilerplate source: https://www.udemy.com/course/learn-the-vulkan-api-with-cpp/

namespace AR
{

/************************* Vulkan Config Struct *************************/


struct VulkanConfig
{
    std::string m_AppName;
    std::vector<const char *> m_ValidationLayers;
    std::vector<const char *> m_Extensions;
    std::vector<const char *> m_DeviceExtensions;

    GLFWwindow *window;

    VulkanConfig(const char *app_name, 
                 const char **extensions, uint32_t ext_count, 
                 const char **validation_layers, uint32_t val_layer_count,
                 const char **device_extensions, uint32_t device_ext_count);

    VulkanConfig(std::string app_name, 
                 std::vector<std::string> extensions,  
                 std::vector<std::string> validation_layers,
                 std::vector<std::string> device_extensions);

};

/************************* Vulkan Context Class *************************/

class VulkanContext
{

public:

    VulkanContext(VulkanConfig &v_config);
    ~VulkanContext();

    static void init(VulkanConfig &v_config);

private:

    static VulkanContext *m_ContextInstance;

    VulkanConfig *m_VukanConfig;

    //
    // Vulkan Components
    //
    VkInstance m_VulkanInstance;
    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_LogicalDevice;
    VkQueue m_GraphicsQueue;
    VkQueue m_PresentationQueue;
    VkSurfaceKHR m_Surface;
    VkSwapchainKHR m_SwapChain;
    std::vector<SwapChainImage> m_SwapChainImages;

    VkFormat m_SwapChainImageFormat;
    VkExtent2D m_SwapChainExtent;


    //
    // Vulkan Init Methods
    //
    void createInstance(VulkanConfig &v_config);
    void createSurface(VulkanConfig &v_config);
    void pickPhysicalDevice();
    void createLogicalDevice(VulkanConfig &v_config);
    void createSwapChain();

    //
    // Support Methods
    //
    bool checkValidationLayerSupport(std::vector<const char *> &validation_layers);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool checkExtensionSupport(std::vector<const char *> &extensions);
    bool isSuitableDevice(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
    VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> &presentation_modes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surface_capabilities);

    QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
    SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

};


}