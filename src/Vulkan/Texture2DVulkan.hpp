#pragma once

#include "../Base/Texture2D.hpp"
#include <vulkan/vulkan.h>

class Texture2DVulkan : public Texture2D {
    public:
        Texture2DVulkan(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkDescriptorPool descriptorPool);
        ~Texture2DVulkan() final;
        
        int loadFromFile(std::string filename) final;
        void bind(unsigned int slot) final;
        
        VkDescriptorSet getDescriptorSet();
        
    private:
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory);
        uint32_t findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void createImageView(VkImage image, VkFormat format, VkImageView* imageView);
        
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        VkDescriptorPool descriptorPool;
        
        VkImage stagingImage;
        VkDeviceMemory stagingImageMemory;
        
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        
        VkImageView textureImageView;
        
        bool descriptorSetCreated = false;
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet descriptorSet;
};
