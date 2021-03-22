#include "ar.h"

#include <iostream>

//
// GLOBALS
//
#define WIDTH 800
#define HEIGHT 600
#define WINDOW_NAME "Vulkan"

int main(int argc, char *argv[])
{
    GLFWwindow *window;
    ar::VulkanConfig v_config;

    // Initialize Window
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, nullptr, nullptr);

    // INITIALIZE VULKAN
    v_config.app_name = "VULKAN APPLICATION";
    v_config.validation_layers.push_back((char *)"VK_LAYER_KHRONOS_validation");
    
	uint32_t glfw_ext_cnt;
	const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_cnt);
	for (int i = 0; i < glfw_ext_cnt; i++)
		v_config.extensions.push_back((char *)glfw_extensions[i]);


    ar::Renderer renderer(v_config);


    // RUN APPLICATION
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }

    // CLEAN UP
    glfwDestroyWindow(window);
    glfwTerminate();

    
    return 0;
}