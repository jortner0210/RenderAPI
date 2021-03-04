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

//
// Utils
//
//typedef struct AR_StringArray

int main() {
	
	//
	// Initialize Window and Vulkan
	//
	GLFWwindow *window;
    initWindow(WIDTH, HEIGHT, WINDOW_NAME, &window);

	AR_Result res;
	ARConfig config;
	config.app_name = "Vulkan App";

	config.val_layer_cnt = 1;
	config.validation_layers 	= (char **)malloc(sizeof(char *) * config.val_layer_cnt);
	config.validation_layers[0] = (char *)malloc(sizeof(char) * AR_EXTENSION_MAX_CHARS);
	//strcpy(config.validation_layers[0], "VK_LAYER_KHRONOS_validation");		// LINUX
	strcpy(config.validation_layers[0], "VK_LAYER_LUNARG_standard_validation"); // WINDOWS
	res = AR_initVulkan(&config);

	//
    // MAIN LOOP
    //
    mainLoop(window);

	//
	// Deallocate Resources
	//
	destroyWindow(window);
	return 0;
}

//
// APPLICATION FUNCTIONS
//

void initWindow(int w, int h, const char *window_name, GLFWwindow **window) 
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    *window = glfwCreateWindow(w, h, window_name, NULL, NULL);
}

void mainLoop(GLFWwindow *window) 
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }
}

void destroyWindow(GLFWwindow *window) 
{
    glfwDestroyWindow(window);

    glfwTerminate();
}
