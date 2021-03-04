#ifndef AR_INSTANCE_H
#define AR_INSTANCE_H

// SOURCE: https://vulkan-tutorial.com/Introduction

#include "Common.h"
#include "Core.h"

//////////////////////////////////////////////////////////////////////////////
//////                        AR INTERNAL API                           //////
//////////////////////////////////////////////////////////////////////////////


static AR_Result _AR_createInstance(
    ARConfig* v_config
);

static AR_Result _AR_checkValidationLayerSupport(
    uint32_t req_layer_count, 
    char **req_val_layers
);

#endif