#include "ar.h"

#include <iostream>

//
// GLOBALS
//
#define WIDTH 800
#define HEIGHT 600
#define WINDOW_NAME "Vulkan Window"

AR::VulkanConfig createVulkanConfig();
GLFWwindow *createWindow(std::string window_name, uint32_t width, uint32_t height);

int main(int argc, char *argv[])
{
    GLFWwindow *window;

    // Initialize window
    window = createWindow(WINDOW_NAME, WIDTH, HEIGHT);

    // Initialize renderer
    AR::VulkanConfig v_config = createVulkanConfig();
    v_config.window = window;
    AR::Renderer renderer(v_config);

    // Run Application
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }

    // Clean up resources
    // /glfwDestroyWindow(window);
    // /glfwTerminate();

    
    return 0;
}

GLFWwindow *createWindow(std::string window_name, uint32_t width, uint32_t height)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    return glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
}

AR::VulkanConfig createVulkanConfig()
{
    std::string app_name("VULKAN APPLICATION");

    uint32_t glfw_ext_cnt, val_layer_count, device_ext_count;
	const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_cnt);

    char *validation_layers[] = {
        (char *)"VK_LAYER_KHRONOS_validation"
    };
    val_layer_count = 1;

    // TO DO - This needs to go somewhere else
    char * device_extensions[] = {
        (char *)VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    device_ext_count = 1;

    std::vector<std::string> extension_vector(glfw_ext_cnt);
    std::vector<std::string> validation_vector(val_layer_count);
    std::vector<std::string> device_extension_vector(device_ext_count);

    // copy extensions to vector
    for (int i = 0; i < glfw_ext_cnt; i++)
        extension_vector[i] = strdup(glfw_extensions[i]);

    // copy validation layers to vector
    for (int i = 0; i < val_layer_count; i++)
        validation_vector[i] = strdup(validation_layers[i]);

    // copy device extensions to vector
    for (int i = 0; i < device_ext_count; i++)
        device_extension_vector[i] = strdup(device_extensions[i]);


    AR::VulkanConfig v_config(app_name, 
                              extension_vector, 
                              validation_vector,
                              device_extension_vector);

    return v_config;
}