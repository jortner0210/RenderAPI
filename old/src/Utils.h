#ifndef AR_UTILS_H
#define AR_UTILS_H

#include "src/Common.h"
#include "src/Core.h"

//
// String Array Utility
//

typedef struct AR_StringArray
{
	uint32_t size;
	uint32_t len;
	char **data;

} AR_StringArray;

AR_Result AR_initStringArray(
    AR_StringArray **arr, 
    uint32_t size
);

AR_Result AR_destroyStringArray(
    AR_StringArray *arr
);

AR_Result AR_appendStringArray(
    AR_StringArray *arr, 
    const char *string
);

AR_Result AR_printStringArray(
    AR_StringArray *arr
);

#endif