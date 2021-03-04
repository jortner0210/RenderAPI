#ifndef AR_CORE_H
#define AR_CORE_H

#include "Common.h"

//////////////////////////////////////////////////////////////////////////////
//////                       AR DATA STRUCTURES                         //////
//////////////////////////////////////////////////////////////////////////////

#define AR_EXTENSION_MAX_CHARS 100

typedef struct ARConfig
{
    const char* app_name;
    char **extensions;
    uint32_t ext_cnt;
    //uint32_t requested_extension_count;
    //const char* const* requested_extensions;

    //uint32_t requested_vlayer_count;
    //const char* const* requested_vlayer_names;

    //GLFWwindow *window;

} ARConfig;

//////////////////////////////////////////////////////////////////////////////
//////                             AR DEBUG                             //////
//////////////////////////////////////////////////////////////////////////////


typedef enum 
{
    AR_SUCCESS,
    AR_ERROR

} AR_Result;


#endif