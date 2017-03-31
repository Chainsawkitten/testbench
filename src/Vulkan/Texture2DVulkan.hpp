#pragma once

#include "../Base/Texture2D.hpp"
#include <vulkan/vulkan.h>

class Texture2DVulkan : public Texture2D {
    public:
        Texture2DVulkan(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
        ~Texture2DVulkan() final;
        
        int loadFromFile(std::string filename) final;
        void bind(unsigned int slot) final;
        
    private:
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory);
        uint32_t findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        
        VkImage stagingImage;
        VkDeviceMemory stagingImageMemory;
        
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
};
