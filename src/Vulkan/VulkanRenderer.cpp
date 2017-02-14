#define VK_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include "VulkanRenderer.hpp"

#include <iostream>
#include <SDL_syswm.h>

#include "ConstantBufferVulkan.hpp"
#include "MaterialVulkan.hpp"
#include "MeshVulkan.hpp"
#include "RenderStateVulkan.hpp"
#include "Sampler2DVulkan.hpp"
#include "Texture2DVulkan.hpp"
#include "VertexBufferVulkan.hpp"

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

#ifndef NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
    std::cerr << "Validation layer: " << msg << std::endl;
    
    return VK_FALSE;
}
#endif

VulkanRenderer::VulkanRenderer() {
    
}

VulkanRenderer::~VulkanRenderer() {

}

Mesh* VulkanRenderer::makeMesh() { 
    return new MeshVulkan();
}

Texture2D* VulkanRenderer::makeTexture2D() {
    return new Texture2DVulkan();
}

Sampler2D* VulkanRenderer::makeSampler2D() {
    return new Sampler2DVulkan();
}

ConstantBuffer* VulkanRenderer::makeConstantBuffer(std::string name, unsigned int location) { 
    return new ConstantBufferVulkan(name, location);
}

std::string VulkanRenderer::getShaderPath() {
    UNIMPLEMENTED
    return "";
}

std::string VulkanRenderer::getShaderExtension() {
    UNIMPLEMENTED
    return "";
}

VertexBuffer* VulkanRenderer::makeVertexBuffer() { 
    return new VertexBufferVulkan();
}

Material* VulkanRenderer::makeMaterial() { 
    return new MaterialVulkan();
}

RenderState* VulkanRenderer::makeRenderState() { 
    return new RenderStateVulkan();
}

int VulkanRenderer::initialize(unsigned int width, unsigned int height) {
    // Create Vulkan instance.
    createInstance();
    setupDebugCallback();
    
    // Init SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(-1);
    }
    
    // Create window.
    window = SDL_CreateWindow("Vulkan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        std::cerr << "Cound not create SDL window." << std::endl;
        exit(-1);
    }
    
    // Create surface to render to.
    SDL_SysWMinfo windowInfo;
    SDL_VERSION(&windowInfo.version);
    SDL_GetWindowWMInfo(window, &windowInfo);
    
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = windowInfo.info.win.hinstance;
    surfaceCreateInfo.hwnd = windowInfo.info.win.window;
    
    if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
        std::cerr << "Failed to create surface." << std::endl;
        exit(-1);
    }
    
    // Create logical device.
    createDevice();
    
    // Create swap chain.
    createSwapChain(width, height);
    
    UNIMPLEMENTED
    return -1;
}

int VulkanRenderer::shutdown() {
    vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    
    SDL_DestroyWindow(window);
    
#ifndef NDEBUG
    auto DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (DestroyDebugReportCallback != nullptr)
        DestroyDebugReportCallback(instance, callback, nullptr);
#endif
    
    vkDestroyInstance(instance, nullptr);
    
    return 0;
}

void VulkanRenderer::setClearColor(float r, float g, float b, float a) {
    UNIMPLEMENTED
}

void VulkanRenderer::clearBuffer(unsigned int flag) {
    UNIMPLEMENTED
}

void VulkanRenderer::setRenderState(RenderState* ps) {
    UNIMPLEMENTED
}

void VulkanRenderer::submit(Mesh* mesh) {
    UNIMPLEMENTED
}

void VulkanRenderer::frame() {
    UNIMPLEMENTED
}

void VulkanRenderer::present() {
    UNIMPLEMENTED
}

void VulkanRenderer::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Testbench";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    
    std::vector<const char*> validationLayers;
#ifndef NDEBUG
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create instance." << std::endl;
        exit(-1);
    }
}

void VulkanRenderer::setupDebugCallback() {
#ifndef NDEBUG
    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = debugCallback;
    
    auto CreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (CreateDebugReportCallbackEXT == nullptr)
        std::cerr << "Failed to get CreateDebugReportCallbackEXT function" << std::endl;
    
    if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
        std::cerr << "Failed to set up debug callback" << std::endl;
#endif
}

void VulkanRenderer::createDevice() {
    uint32_t deviceCount = 0;
    
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        std::cerr << "Failed to find GPU with Vulkan support." << std::endl;
        exit(-1);
    }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    for (const auto& device : devices){
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        // Checking for discrete (dedicated) GPU.
        /// @todo Check for actually necessary GPU features.
        // Maybe: Wrap this in a function that takes as argument the things we are looking for.
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            std::cout << "Found suitable GPU." << std::endl;
            physicalDevice = device;
            break;
        }
    }
    
    if (physicalDevice == VK_NULL_HANDLE)
        std::cerr << "Failed to find suitable GPU's." << std::endl;
    
    int graphicsFamily = -1;
    int presentFamily = -1;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    
    // Check for available queue families.
    int i = 0;
    for (const VkQueueFamilyProperties& queueFamily : queueFamilies){
        if (queueFamily.queueCount > 0) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphicsFamily = i;
            
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (presentSupport)
                presentFamily = i;
        }
        
        if (graphicsFamily >= 0 && presentFamily >= 0)
            break;
        ++i;
    }
    
    std::cout << "Found " << queueFamilyCount << " queue families." << std::endl;
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = { graphicsFamily, presentFamily };
    
    // Queue priority between 0-1.
    float queuePriority = 1.0f;
    for (int queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    // Device extensions.
    std::vector<const char*> deviceExtensions;
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    
    /// @todo Check if the extensions are actually supported.
    
    // Device features.
    VkPhysicalDeviceFeatures deviceFeatures = {};
    
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
        std::cerr << "Could not create logical device." << std::endl;
    else
        std::cout << "Logical device created." << std::endl;
    
    vkGetDeviceQueue(logicalDevice, graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, presentFamily, 0, &presentQueue);
}

VulkanRenderer::SwapChainSupport VulkanRenderer::querySwapChainSupport() {
    SwapChainSupport swapChainSupport;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainSupport.capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    swapChainSupport.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainSupport.formats.data());
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    swapChainSupport.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, swapChainSupport.presentModes.data());
    
    return swapChainSupport;
}

VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    
    for (const VkSurfaceFormatKHR& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    }
    
    return availableFormats[0];
}

void VulkanRenderer::createSwapChain(unsigned int width, unsigned int height) {
    // Determine swap chain support.
    SwapChainSupport swapChainSupport = querySwapChainSupport();
    
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    
    /// @todo Choose present mode based on swap chain support.
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    
    /// @todo Choose extent based on swap chain support.
    VkExtent2D extent = {width, height};
    
    /// @todo Choose image count based on swap chain support.
    uint32_t imageCount = 2;
    
    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    /// @todo Handle case where graphics queue is not the same as the present queue.
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.queueFamilyIndexCount = 0;
    swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    
    /// @todo Determine pretransform based on current transform in swap chain.
    swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    
    if (vkCreateSwapchainKHR(logicalDevice, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS) {
        std::cerr << "Failed to create swap chain." << std::endl;
        exit(-1);
    }
}
