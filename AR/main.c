#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "ar.h"

#include <stdio.h>
#include <errno.h> 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//
// GLOBALS
//
#define WIDTH 800
#define HEIGHT 600
#define WINDOW_NAME "Vulkan"

//
// APPLICATION FUNCTIONS
//
void initWindow(int w, int h, const char *window_name, GLFWwindow **window);
void mainLoop(GLFWwindow *window);
void destroyWindow(GLFWwindow *window);

int main() {
	
	//
	// Initialize Window and Vulkan
	//
	GLFWwindow *window;
    initWindow(WIDTH, HEIGHT, WINDOW_NAME, &window);

	// Validation
	AR_StringArray *validation_layers;
	AR_initStringArray(&validation_layers, 1);
	AR_appendStringArray(validation_layers, "VK_LAYER_KHRONOS_validation");	// LINUX
	//AR_appendStringArray(validation_layers, "VK_LAYER_LUNARG_standard_validation"); // WINDOWS

	// Extensions
	AR_StringArray *extensions;
	uint32_t glfw_ext_cnt;
	const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_cnt);
	AR_initStringArray(&extensions, glfw_ext_cnt);
	for (int i = 0; i < glfw_ext_cnt; i++)
		AR_appendStringArray(extensions, glfw_extensions[i]);	

	// Create config
	AR_Result res;
	ARConfig config;
	config.app_name = "Vulkan App";

	// Set Validation layers
	config.val_layer_cnt 	 = validation_layers->len;
	config.validation_layers = validation_layers->data;

	// Set extensions
	config.ext_cnt 	  = extensions->len;
	config.extensions = extensions->data;

	res = AR_initVulkan(&config);

	//
    // MAIN LOOP
    //
    mainLoop(window);

	//
	// Deallocate Resources
	//
	//destroyWindow(window);

	//AR_destroyStringArray(validation_layers);
	//AR_destroyStringArray(extensions);
	return 0;
}

//
// APPLICATION FUNCTIONS
//

void initWindow(
	int w, 
	int h, 
	const char *window_name, 
	GLFWwindow **window
) 
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    *window = glfwCreateWindow(w, h, window_name, NULL, NULL);
}

void mainLoop(
	GLFWwindow *window
) 
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }
}

void destroyWindow(
	GLFWwindow *window
) 
{
    glfwDestroyWindow(window);

    glfwTerminate();
}