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
// AR UTILS
//

// Can be useful 
typedef struct AR_StringArray
{
	uint32_t size;
	uint32_t len;
	char **data;

} AR_StringArray;

AR_Result AR_initStringArray(AR_StringArray **arr, uint32_t size);
AR_Result AR_destroyStringArray(AR_StringArray *arr);
AR_Result AR_appendStringArray(AR_StringArray *arr, const char *string);
AR_Result AR_printStringArray(AR_StringArray *arr);

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
	destroyWindow(window);

	AR_destroyStringArray(validation_layers);
	AR_destroyStringArray(extensions);
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

//
// AR UTILS
//
AR_Result AR_initStringArray(
	AR_StringArray **arr, 
	uint32_t size
)
{
	(*arr) = (AR_StringArray *)malloc(sizeof(AR_StringArray));
	(*arr)->size = size;
	(*arr)->len  = 0;
	(*arr)->data  = (char **)malloc(sizeof(char *) * size);
	for (int i = 0; i < size; i++)
		(*arr)->data[i] = NULL;

	return AR_SUCCESS;
}

AR_Result AR_destroyStringArray(
	AR_StringArray *arr
)
{
	for (int i = 0; i < arr->len; i++) 
		if (arr->data[i] != NULL) 
			free(arr->data[i]);
}

AR_Result AR_appendStringArray(
	AR_StringArray *arr, 
	const char *string
)
{
	if (arr->len == arr->size) {
		printf("Can't add more items to array");
		return AR_ERROR;
	}
	arr->data[arr->len] = (char *)malloc(sizeof(char) * sizeof(string));
	strcpy(arr->data[arr->len], (char *)string);
	arr->len++;
}

AR_Result AR_printStringArray(
	AR_StringArray *arr
)
{
	for (int i = 0; i < arr->len; i++) 
		printf("idx: %d, string: %s\n", i, arr->data[i]);
}