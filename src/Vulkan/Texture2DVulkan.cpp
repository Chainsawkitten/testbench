#include "Texture2DVulkan.hpp"

#include <iostream>
#include <stb_image.h>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

Texture2DVulkan::Texture2DVulkan(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue) {
    this->logicalDevice = logicalDevice;
    this->physicalDevice = physicalDevice;
    this->commandPool = commandPool;
    this->graphicsQueue = graphicsQueue;
}

Texture2DVulkan::~Texture2DVulkan() {
    vkFreeMemory(logicalDevice, stagingImageMemory, nullptr);
    vkDestroyImage(logicalDevice, stagingImage, nullptr);
    
    vkFreeMemory(logicalDevice, textureImageMemory, nullptr);
    vkDestroyImage(logicalDevice, textureImage, nullptr);
}

int Texture2DVulkan::loadFromFile(std::string filename) {
    // Load texture from file.
    int width, height, channels;
    stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    VkDeviceSize imageSize = width * height * 4;
    if (pixels == nullptr) {
        std::cerr << "Failed to load image " << filename << std::endl;
        exit(-1);
    }
    
    // Create staging image.
    createImage(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingImage, &stagingImageMemory);
    
    // Transfer image data.
    void* data;
    vkMapMemory(logicalDevice, stagingImageMemory, 0, imageSize, 0, &data);
    
    VkImageSubresource subresource = {};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel = 0;
    subresource.arrayLayer = 0;
    
    VkSubresourceLayout stagingImageLayout;
    vkGetImageSubresourceLayout(logicalDevice, stagingImage, &subresource, &stagingImageLayout);
    
    if (stagingImageLayout.rowPitch == width * 4u) {
        memcpy(data, pixels, imageSize);
    } else {
        uint8_t* dataBytes = reinterpret_cast<uint8_t*>(data);
        
        for (int y = 0; y < height; ++y)
            memcpy(&dataBytes[y * stagingImageLayout.rowPitch], &pixels[y * width * 4], width * 4);
    }
    
    vkUnmapMemory(logicalDevice, stagingImageMemory);
    
    // Clean up.
    stbi_image_free(pixels);
    
    // Create texture image.
    createImage(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &textureImage, &textureImageMemory);
    
    // Prepare the texture image.
    transitionImageLayout(stagingImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyImage(stagingImage, textureImage, width, height);
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    
    return 0;
}

void Texture2DVulkan::bind(unsigned int slot) {
    UNIMPLEMENTED
}

void Texture2DVulkan::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory) {
    // Create image.
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;
    
    if (vkCreateImage(logicalDevice, &imageInfo, nullptr, image) != VK_SUCCESS) {
        std::cerr << "Failed to create image." << std::endl;
        exit(-1);
    }
    
    // Get memory requirements.
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(logicalDevice, *image, &memoryRequirements);
    
    // Allocate memory.
    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);
    
    if (vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, imageMemory)) {
        std::cerr << "Failed to allocate image memory." << std::endl;
        exit(-1);
    }
    
    vkBindImageMemory(logicalDevice, *image, *imageMemory, 0);
}

uint32_t Texture2DVulkan::findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    uint32_t memoryType;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (memoryTypeBits & (1u << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties )
            memoryType = i;
    }
    
    return memoryType;
}

void Texture2DVulkan::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0; /// @todo
    barrier.dstAccessMask = 0; /// @todo
    
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    
    endSingleTimeCommands(commandBuffer);
}

void Texture2DVulkan::copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    
    VkImageSubresourceLayers subResource = {};
    subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subResource.baseArrayLayer = 0;
    subResource.mipLevel = 0;
    subResource.layerCount = 1;
    
    VkImageCopy region = {};
    region.srcSubresource = subResource;
    region.dstSubresource = subResource;
    region.srcOffset = {0, 0, 0};
    region.dstOffset = {0, 0, 0};
    region.extent.width = width;
    region.extent.height = height;
    region.extent.depth = 1;
    
    vkCmdCopyImage(commandBuffer, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    
    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer Texture2DVulkan::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = commandPool;
    allocateInfo.commandBufferCount = 1;
    
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &commandBuffer);
    
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    return commandBuffer;
}

void Texture2DVulkan::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
    
    vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}
