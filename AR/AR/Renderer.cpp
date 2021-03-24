#include "Renderer.h"

namespace AR
{

Renderer::Renderer(VulkanConfig &v_config)
{
    VulkanContext::init(v_config);
}

}