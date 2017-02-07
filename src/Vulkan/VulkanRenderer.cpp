#include "VulkanRenderer.hpp"

#include <iostream>
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
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Testbench";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    const char* extensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME };
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensionNames;
    createInfo.enabledLayerCount = 0;
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cout << "Failed to create instance." << std::endl;
        exit(-1);
    }
    
    // Init SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(-1);
    }
    
    // Create window.
    window = SDL_CreateWindow("Vulkan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0)
        std::cout << "Failed to find GPU's with Vulkan support." << std::endl;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for(const auto& device : devices){
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        //Checking for discrete (dedicated) GPU.
        //TODO: Check for actually necessary GPU features.
        //Maybe: Wrap this in a function that takes as argument the things we are looking for.
        if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            std::cout << "Found suitable GPU." << std::endl;
            physicalDevice = device;
            break;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE)
        std::cout << "Failed to find suitable GPU's." << std::endl;

    int graphicsFamily = -1;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    //Check for available queue families.
    int i = 0;
    for(const auto& queueFamily : queueFamilies){
        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            graphicsFamily = i;

        if(graphicsFamily >= 0)
            break;
        i++;
    }

    std::cout << "Found " << queueFamilyCount << " queue families." << std::endl;

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsFamily;
    queueCreateInfo.queueCount = 1;

    //Queue priority between 0.0f - 1.0f
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    //Device features.
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    if(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
        std::cout << "Could not create logical device." << std::endl;
    else
        std::cout << "Logical device created." << std::endl;
    vkGetDeviceQueue(logicalDevice, graphicsFamily, 0, &graphicsQueue);

    UNIMPLEMENTED
    return -1;
}

int VulkanRenderer::shutdown() {
    vkDestroyDevice(logicalDevice, nullptr);

    SDL_DestroyWindow(window);
    
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
