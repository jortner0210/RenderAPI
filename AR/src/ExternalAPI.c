#include "ExternalAPI.h"
#include "Instance.h"
#include "Common.h"
#include "Core.h"

#include <vulkan/vulkan.h>

AR_Result AR_initVulkan(
    ARConfig* v_config
)
{
    _AR_createInstance(v_config);
    return AR_SUCCESS;
}

AR_Result AR_destroyVulkan()
{

}


