#pragma once

#include "../Base/Texture2D.hpp"
#include <vulkan/vulkan.h>

class Texture2DVulkan : public Texture2D {
    public:
        Texture2DVulkan(VkDevice logicalDevice, VkPhysicalDevice physicalDevice);
        ~Texture2DVulkan() final;
        
        int loadFromFile(std::string filename) final;
        void bind(unsigned int slot) final;
        
    private:
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory);
        uint32_t findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
        
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice;
        VkImage stagingImage;
        VkDeviceMemory stagingImageMemory;
};
