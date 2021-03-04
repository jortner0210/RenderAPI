#include "src/Utils.h"

//
// String Array Utility
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
	arr->data[arr->len] = (char *)malloc(sizeof(char) * (strlen(string) + 1));
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