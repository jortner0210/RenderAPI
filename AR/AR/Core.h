#pragma once

//////////////////////////////////////////////////////////////////////////////
//////                             AR DEBUG                             //////
//////////////////////////////////////////////////////////////////////////////
namespace ar
{

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

#define AR_DEBUG_TRACE_FMT(fmt, ...)                         \
    do {                                                     \
        if (DEBUG_TEST) {                                    \
            printf("%s", KGRN);                              \
            printf("AR DEBUG TRACE -- %s:%d:%s() -- %s" fmt, \
                   __FILE__,                                 \
                   __LINE__,                                 \
                   __func__,                                 \
                   KYEL,                                     \
                   __VA_ARGS__);                             \
            printf("%s\n", KNRM);                            \
        }                                                    \
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

}