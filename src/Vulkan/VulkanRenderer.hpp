#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>
#include "../Base/Renderer.hpp"

class VulkanRenderer : public Renderer {
    public:
        VulkanRenderer();
        ~VulkanRenderer();
        
        Material* makeMaterial();
        Mesh* makeMesh();
        VertexBuffer* makeVertexBuffer();
        ConstantBuffer* makeConstantBuffer(std::string name, unsigned int location);
        RenderState* makeRenderState();
        Texture2D* makeTexture2D();
        Sampler2D* makeSampler2D();
        std::string getShaderPath();
        std::string getShaderExtension();
        
        int initialize(unsigned int width = 640, unsigned int height = 480);
        int shutdown();
        
        void setClearColor(float, float, float, float);
        void clearBuffer(unsigned int);
        void setRenderState(RenderState* ps);
        void submit(Mesh* mesh);
        void frame();
        void present();
        
    private:
        struct SwapChainSupport {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };
        
        void createInstance();
        void setupDebugCallback();
        void createDevice();
        
        SwapChainSupport querySwapChainSupport();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height);
        VkFormat createSwapChain(unsigned int width, unsigned int height);
        
        void createSemaphores();
        void createImageViews(VkFormat format);
        void createRenderPass(VkFormat format);
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffer();
        void createDescriptorPool();
        void createDepthBuffer();
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        
        std::vector<Mesh*> drawList;
        
        SDL_Window* window;
        
        VkInstance instance;
#ifndef NDEBUG
        VkDebugReportCallbackEXT callback;
#endif

        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice;
        int graphicsFamily;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        VkExtent2D swapChainExtent;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
        VkDescriptorPool descriptorPool;
        VkClearValue clearColor;
        
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        // Current swap chain index to render to.
        uint32_t imageIndex;
        VkFence fence;
};
