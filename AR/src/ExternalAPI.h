#ifndef AR_EXTERNAL_API_H
#define AR_EXTERNAL_API_H

#include <GLFW/glfw3.h>

#include "InternalAPI.h"

//////////////////////////////////////////////////////////////////////////////
//////                       LVL EXTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////

typedef struct LVLConfig
{
    const char* app_name;
    uint32_t requested_extension_count;
    const char* const* requested_extensions;

    uint32_t requested_vlayer_count;
    const char* const* requested_vlayer_names;

    GLFWwindow *window;

} LVLConfig;

int  LVL_InitVulkan(LVLConfig* init);
void LVL_CleanupVulkan();

#endif