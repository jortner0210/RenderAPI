#pragma once


namespace AR
{

    /************************* Queue Family Indices *************************/

    struct QueueFamilyIndices
    {
        int m_GraphicsFamily     = -1;      // Location of Graphics Queue Family
        int m_PresentationFamily = -1;  // Location of Presentation Queue Family

        /**
         * @brief Check that all queue families are valid
         * 
         * @return true: queue families are valid
         * @return false: queue families are not valid 
         */
        bool isValid()
        {
            return m_GraphicsFamily >= 0 && m_PresentationFamily >= 0;
        }
    };

    /************************* Swap Chain Details *************************/

    struct SwapChainDetails 
    {
        VkSurfaceCapabilitiesKHR m_SurfaceCapabilities;    // Surface properties (i.e. image size/extent)
        std::vector<VkSurfaceFormatKHR> m_Formats;         // Vector of supported image formats (i.e. RGBA and size of each color)
        std::vector<VkPresentModeKHR> m_PresentationModes; // Supported surface presentation modes
    };
}