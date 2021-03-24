

#include "VulkanContext.h"
#include "Core.h"

namespace AR 
{

/**
 * @brief Vulkan Config Struct Constructor
 * 
 * @param app_name 
 * @param extensions 
 * @param ext_count 
 * @param validation_layers 
 * @param val_layer_count 
 */
VulkanConfig::VulkanConfig(const char *app_name, 
             const char **extensions, uint32_t ext_count, 
             const char **validation_layers, uint32_t val_layer_count,
             const char **device_extensions, uint32_t device_ext_count) :
    m_AppName(app_name), m_Extensions(ext_count), m_ValidationLayers(val_layer_count), m_DeviceExtensions(device_ext_count)
{
    // copy extensions to vector
    for (int i = 0; i < ext_count; i++)
        m_Extensions[i] = strdup(extensions[i]);
    // copy validation layers to vector
    for (int i = 0; i < val_layer_count; i++)
        m_ValidationLayers[i] = strdup(validation_layers[i]);
    // copy device extensions to vector
    for (int i = 0; i < device_ext_count; i++)
        m_DeviceExtensions[i] = strdup(device_extensions[i]);
}

/**
 * @brief Vulkan Config Struct Constructor
 * 
 * @param app_name: String application name
 * @param extensions: Vector of extensions 
 * @param validation_layers: Vector of validation layers 
 */
VulkanConfig::VulkanConfig(std::string app_name, 
                           std::vector<std::string> extensions,  
                           std::vector<std::string> validation_layers,
                           std::vector<std::string> device_extensions) :
    m_AppName(app_name), m_Extensions(extensions.size()), m_ValidationLayers(validation_layers.size()), m_DeviceExtensions(device_extensions.size())
{
    int i = 0;
    for (std::string &ext : extensions) {
        m_Extensions[i] = strdup(ext.c_str());
        i++;
    }

    i = 0;
    for (std::string &val : validation_layers) {
        m_ValidationLayers[i] = strdup(val.c_str());
        i++;
    }

    i = 0;
    for (std::string &d_ext : device_extensions) {
        m_DeviceExtensions[i] = strdup(d_ext.c_str());
        i++;
    }
}


//
// Vulkan Instance Singleton
//
VulkanContext *VulkanContext::m_ContextInstance = nullptr;

/************************* Vulkan Context Class Public Methods *************************/

/**
 * @brief Initialize the Vulkan Context
 * 
 * @param v_config: Vulkan Configuration Struct
 */
void VulkanContext::init(VulkanConfig &v_config)
{

    if (m_ContextInstance == nullptr)
        m_ContextInstance = new VulkanContext(v_config);
    
}

/************************* Vulkan Context Class Private Methods *************************/

/**
 * @brief Vulkan Context Constructor
 * 
 * @param v_config: Vulkan Config Struct
 */
VulkanContext::VulkanContext(VulkanConfig &v_config) :
    m_VukanConfig(&v_config)
{
    createInstance(v_config);

    createSurface(v_config);

    pickPhysicalDevice();

    createLogicalDevice(v_config);

    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "|--- VULKAN INITIALIZATION COMPLETED ---|")
}

/**
 * @brief Vulkan Context Destructor
 * 
 */
VulkanContext::~VulkanContext()
{
    vkDestroyDevice(m_LogicalDevice, nullptr);
    vkDestroyInstance(m_VulkanInstance, nullptr);
}

/**
 * @brief Create a Vulkan Instance
 * 
 * @param v_config: Vulkan Config Struct 
 */
void VulkanContext::createInstance(VulkanConfig &v_config)
{
    if (AR_ENABLE_VALIDATION_LAYERS) 
        if (!checkValidationLayerSupport(v_config.m_ValidationLayers)) 
            AR_EXIT_FAILURE("Requested Validation layer not supported!")

    if (!checkExtensionSupport(v_config.m_Extensions)) 
        AR_EXIT_FAILURE("Requested extension not supported!")

    // Application Information
    VkApplicationInfo app_info = {};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext              = nullptr;
    app_info.pApplicationName   = "Vulkan Program Template";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "No Engine";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = VK_API_VERSION_1_2;

    // Vulkan Instance Information 
    VkInstanceCreateInfo inst_info = {};
    inst_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext                   = nullptr;
    inst_info.flags                   = 0;
    inst_info.pApplicationInfo        = &app_info;
    inst_info.enabledExtensionCount   = (uint32_t)v_config.m_Extensions.size();
    inst_info.ppEnabledExtensionNames = (const char * const *)v_config.m_Extensions.data();

    if (AR_ENABLE_VALIDATION_LAYERS) {
		inst_info.enabledLayerCount = (uint32_t)v_config.m_ValidationLayers.size();
        inst_info.ppEnabledLayerNames = (const char * const *)v_config.m_ValidationLayers.data();
    }
    else {
        inst_info.enabledLayerCount = 0;
        inst_info.ppEnabledLayerNames = nullptr;
    }

    if (vkCreateInstance(&inst_info, nullptr, &m_VulkanInstance) != VK_SUCCESS) {
        AR_EXIT_FAILURE("Failed to create Vulkan Instance!")
    }
    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "Vulkan Instance Created!")
}

void VulkanContext::createSurface(VulkanConfig &v_config)
{

    if (glfwCreateWindowSurface(m_VulkanInstance, v_config.window, nullptr, &m_Surface) != VK_SUCCESS) {
        AR_EXIT_FAILURE("Failed to create window surface!")
    }
    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "Surface Registered!")

// NOTE: Will be handled this way when not using GLFW
//#ifdef __linux__
//#elif _WIN32
//    VkWin32SurfaceCreateInfoKHR create_info = {};
//    create_info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//    create_info.hwnd      = glfwGetWin32Window(v_config.window);
//    create_info.hinstance = GetModuleHandle(nullptr);
//    if (vkCreateWin32SurfaceKHR(m_VulkanInstance, &create_info, nullptr, &m_Surface) != VK_SUCCESS) {
//        AR_EXIT_FAILURE("Failed to create window surface!")
//    }
//    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "Surface Registered!")
//#endif
}

void VulkanContext::pickPhysicalDevice()
{
    m_PhysicalDevice = VK_NULL_HANDLE;

    uint32_t device_count;
    vkEnumeratePhysicalDevices(m_VulkanInstance, &device_count, nullptr);
    AR_DEBUG_TRACE_FMT(AR_DB_OKAY, "Physical Devices Found: %d", device_count)
    if (!device_count)
        AR_EXIT_FAILURE("Failed to find GPUs with Vulkan Support!")

    std::vector<VkPhysicalDevice> physical_devices(device_count);
    vkEnumeratePhysicalDevices(m_VulkanInstance, &device_count, physical_devices.data());

    // Iterate through devices to find one that is suitable
    for (int i = 0; i < device_count; i++) {
        if (isSuitableDevice(physical_devices[i])) {
            AR_DEBUG_TRACE_FMT(AR_DB_GOOD, "Suitable device found! Device Number: %d", i);
            m_PhysicalDevice = physical_devices[i];
            break;
        }
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        AR_EXIT_FAILURE("Failed to find a suitable device!");
    }
    
}

void VulkanContext::createLogicalDevice(VulkanConfig &v_config)
{
    // Get queue family indices for picked physical device
    QueueFamilyIndices indices = getQueueFamilies(m_PhysicalDevice);

    // Vector for queue creation information and set for family indices
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<int> queue_family_indices = { 
        indices.m_GraphicsFamily,
        indices.m_PresentationFamily
    };
    
    // Queues the logical device needs to create and info to do so
    for (int queue_family_idx : queue_family_indices) {

        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family_idx; // The index of the family to create a queue from
        queue_create_info.queueCount       = 1;
        
        float priority = 1.0f;
        queue_create_info.pQueuePriorities = &priority; // Priority if multiple queues (1 = highest priority)

        queue_create_infos.push_back(queue_create_info);

    }
    
    // Information used to create LOGICAL DEVICE
    VkDeviceCreateInfo device_create_info = {};
    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.queueCreateInfoCount    = (uint32_t)queue_create_infos.size();
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();  // List of queue create infos for device to create
    device_create_info.enabledExtensionCount   = (uint32_t)m_VukanConfig->m_DeviceExtensions.size();//(uint32_t)v_config.m_DeviceExtensions.size();                          // Device extensions, not to be confused with Vulkan Instance extensions
    device_create_info.ppEnabledExtensionNames = m_VukanConfig->m_DeviceExtensions.data();
    
    // Set the physical device features that the logical device will use
    VkPhysicalDeviceFeatures device_features = {};
    device_create_info.pEnabledFeatures = &device_features;   

    if (vkCreateDevice(m_PhysicalDevice, &device_create_info, nullptr, &m_LogicalDevice) != VK_SUCCESS) {
        AR_EXIT_FAILURE("Failed to create Logical Device!")
    }
    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "Vulkan Logical Device Created!")

    // Set up access to the queues created with the logical device

    // Store a handle to graphics queue. Logical Device -> Queue Family -> Queue Index (0 since only one queue)
    vkGetDeviceQueue(m_LogicalDevice, indices.m_GraphicsFamily, 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_LogicalDevice, indices.m_PresentationFamily, 0, &m_PresentationQueue);
}

/**
 * @brief Checks that all requested validation layers are available
 * 
 * @param validation_layers: Vector of char pointers
 * @return true: all validation layers are supported
 * @return false: not all validation layers are supported
 */
bool VulkanContext::checkValidationLayerSupport(std::vector<const char *> &validation_layers)
{
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);
    AR_DEBUG_TRACE_FMT(AR_DB_OKAY, "Supported Validation Layers: %d", layer_count)

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    // Check that all the requested layers appear in the available layers
    for (const char *layer : validation_layers) {
        bool layer_found = false;

        for (VkLayerProperties &layer_prop : available_layers) {
            if (strcmp(layer, layer_prop.layerName) == 0) {
                AR_DEBUG_TRACE_FMT(AR_DB_GOOD, "Validation Layer Supported: %s", layer)
                layer_found = true;
                break;
            }
        }
        if (!layer_found)
            return false;
        
    }
    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "All Requested Validation Layers Supported!")
    return true;
}

bool VulkanContext::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    // Retreive device extensions
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    AR_DEBUG_TRACE_FMT(AR_DB_GOOD, "Vulkan Device Extensions Found: %d", extension_count)

    if (!extension_count) {
        return false;
        AR_DEBUG_TRACE_ARG(AR_DB_ERROR, "Vulkan Device Extensions Found")
    }

    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extensions.data());

    // Check for required extensions
    for (const char *device_ext : m_VukanConfig->m_DeviceExtensions) {
        bool has_extension = false;

        for (VkExtensionProperties &ext : extensions) {
            if (strcmp(device_ext, ext.extensionName) == 0) {
                AR_DEBUG_TRACE_FMT(AR_DB_GOOD, "Device Extension Supported: %s", device_ext)
                has_extension = true;
                break;
            }
        }
    
        if (!has_extension)
            return false;
    }
    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "All Device Requested Extensions Supported!")
    return true;
}

bool VulkanContext::checkExtensionSupport(std::vector<const char *> &extensions)
{
    uint32_t extension_count;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    AR_DEBUG_TRACE_FMT(AR_DB_OKAY, "Supported Extensions: %d", extension_count)

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());
    
    // Check that all the requested extensions are available
    for (const char *extension : extensions) {
        bool ext_found = false;

        for (VkExtensionProperties &ext_prop: available_extensions) {
            if (strcmp(extension, ext_prop.extensionName) == 0) {
                AR_DEBUG_TRACE_FMT(AR_DB_GOOD, "Extension Supported: %s", extension)
                ext_found = true;
                break;
            }
        }
        if (!ext_found)
            return false;
        
    }
    AR_DEBUG_TRACE_ARG(AR_DB_GOOD, "All Requested Extensions Supported!")
    return true;
}

bool VulkanContext::isSuitableDevice(VkPhysicalDevice device)
{
    // Information about the device (ID, name, type, vender, etc...)
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);

    // Information about what the device can do (geo shader, tess shader, wide lines, etc...)
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    // Queue Families
    QueueFamilyIndices queue_indices = getQueueFamilies(device);

    // Device Extensions 
    bool extensions_supported = checkDeviceExtensionSupport(device);

    // Swap Chain support
    SwapChainDetails swap_chain_details = getSwapChainDetails(device);
    bool swap_chain_vaild = !swap_chain_details.m_PresentationModes.empty() && !swap_chain_details.m_Formats.empty();

    return queue_indices.isValid() && extensions_supported && swap_chain_vaild;
}

QueueFamilyIndices VulkanContext::getQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
    AR_DEBUG_TRACE_FMT(AR_DB_OKAY, "Queue Family Properties Found: %d", queue_family_count)

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    // Go through each queue family and check if it has at least one of the required types of queue
    int i = 0;
    for (VkQueueFamilyProperties &queue_family : queue_families) {

        // First check if queue family has at lease one queue (might have no queues)
        // Check for graphics support.
        if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            AR_DEBUG_TRACE_ARG(AR_DB_OKAY, "Queue family support found: _GRAPHICS_")
            indices.m_GraphicsFamily = i;
        }

        // Check if queue family supports presentation
        VkBool32 presentation_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentation_support);
        if (queue_family.queueCount > 0 && presentation_support) {
            AR_DEBUG_TRACE_ARG(AR_DB_OKAY, "Queue family support found: _PRESENTATION_")
            indices.m_PresentationFamily = i;
        }

        // Stop searching if queue family indices are in a valid state.
        if (indices.isValid())
            break;

        i++;
    }

    return indices;
}

/**
 * @brief For a given device check support for:
 *          1) Surface Capabilities
 *          2) Image Formats
 *          3) Presentation Modes
 * 
 * @param device 
 * @return SwapChainDetails 
 */
SwapChainDetails VulkanContext::getSwapChainDetails(VkPhysicalDevice device)
{
    SwapChainDetails swap_chain_details;

    // Device Surface Capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &swap_chain_details.m_SurfaceCapabilities); 

    // Formats
    uint32_t format_count(0);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &format_count, nullptr);

    if (format_count) {
        swap_chain_details.m_Formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &format_count, swap_chain_details.m_Formats.data());
    }

    // Presentation Modes
    uint32_t presentation_count(0);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentation_count, nullptr);

    if (presentation_count) {
        swap_chain_details.m_PresentationModes.resize(presentation_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentation_count, swap_chain_details.m_PresentationModes.data());
    }

    return swap_chain_details;
}

}