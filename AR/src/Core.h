#ifndef AR_CORE_H
#define AR_CORE_H

#include "src/Common.h"

//////////////////////////////////////////////////////////////////////////////
//////                       AR DATA STRUCTURES                         //////
//////////////////////////////////////////////////////////////////////////////

#define AR_EXTENSION_MAX_CHARS 100

typedef struct ARConfig
{
    const char* app_name;
    
    // Validation Layers
    char **validation_layers;
    uint32_t val_layer_cnt;

    // Extensions
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
    AR_ERROR,

    AR_VALIDATION_LAYER_NOT_FOUND

} AR_Result;


#ifdef NDEBUG
    #define DEBUG_TEST 0
    #define AR_ENABLE_VALIDATION_LAYERS 0
#else
    #define DEBUG_TEST 1
    #define AR_ENABLE_VALIDATION_LAYERS 1
#endif

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define AR_DEBUG_TRACE_FMT(fmt, ...)                        \
    do {                                                    \
        if (DEBUG_TEST) {                                   \
            printf("%s", KGRN);                             \
            printf("AR DEBUG TRACE -- %s:%d:%s() -- " fmt, \
                   __FILE__,                                \
                   __LINE__,                                \
                   __func__,                                \
                   __VA_ARGS__);                            \
            printf("%s\n", KNRM);                           \
        }                                                   \
    } while (0);

#define AR_DEBUG_TRACE_ARG(arg)\
    AR_DEBUG_TRACE_FMT("%s", arg)

#define AR_EXIT_FAILURE(msg)                             \
    do {                                                 \
        printf("%s", KRED);                              \
        printf("AR EXIT FAILURE -- %s:%d:%s() -- " msg, \
               __FILE__,                                 \
               __LINE__,                                 \
               __func__);                                \
        printf("%s\n", KNRM);                            \
        exit(EXIT_FAILURE);                              \
    } while (0);  


//////////////////////////////////////////////////////////////////////////////
//////                             AR UTILS                             //////
//////////////////////////////////////////////////////////////////////////////

typedef enum AR_Boolean
{
    AR_TRUE,
    AR_FALSE

} AR_Boolean;


#endif