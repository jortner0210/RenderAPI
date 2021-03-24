#pragma once

namespace AR
{

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

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

#define AR_DB_DEBUG   KBLU
#define AR_DB_GOOD    KGRN
#define AR_DB_OKAY    KYEL
#define AR_DB_ERROR   KRED
#define AR_DB_GENERAL KWHT

#define AR_DEBUG_TRACE_FMT(lvl, fmt, ...)                    \
    do {                                                     \
        if (DEBUG_TEST) {                                    \
            printf("%s", AR_DB_DEBUG);                              \
            printf("AR DEBUG: " AR_DB_GENERAL "[%s:%d:%s()] -- %s" fmt, \
                   __FILENAME__,                                 \
                   __LINE__,                                 \
                   __func__,                                 \
                   lvl,                                     \
                   __VA_ARGS__);                             \
            printf("%s\n", KNRM);                            \
        }                                                    \
    } while (0);

#define AR_DEBUG_TRACE_ARG(lvl, arg)\
    AR_DEBUG_TRACE_FMT(lvl, "%s", arg)

#define AR_EXIT_FAILURE(msg)                             \
    do {                                                 \
        printf("%s", AR_DB_ERROR);                              \
        printf("AR EXIT FAILURE -- %s:%d:%s() -- " msg, \
               __FILENAME__,                                 \
               __LINE__,                                 \
               __func__);                                \
        printf("%s\n", KNRM);                            \
        exit(EXIT_FAILURE);                              \
    } while (0);  

}