#pragma once

#include "Common.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace ar 
{

struct VulkanConfig
{
    std::string app_name;
    std::vector<char *> validation_layers;
    std::vector<char *> extensions;

    GLFWwindow *window;
};

class Renderer {

public:

    Renderer(VulkanConfig &v_config);

private:

};


class VulkanContext
{

public:

    VulkanContext(VulkanConfig &v_config);

    static void init(VulkanConfig &v_config);

private:

    static VulkanContext *m_Instance;

    VkInstance m_VulkanInstance;

    //
    // Vulkan Init Methods
    //
    void createInstance(VulkanConfig &v_config);
    void createSurface(VulkanConfig &v_config);
    void pickPhysicalDevice();
    void createLogicalDevice(VulkanConfig &v_config);

    //
    // Vulkan Instance Methods
    //
    bool checkValidationLayerSupport(std::vector<char *>  validation_layers);

};


}