#include "Texture2DVulkan.hpp"

#include <iostream>
#include <stb_image.h>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

Texture2DVulkan::Texture2DVulkan(VkDevice logicalDevice, VkPhysicalDevice physicalDevice) {
    this->logicalDevice = logicalDevice;
    this->physicalDevice = physicalDevice;
}

Texture2DVulkan::~Texture2DVulkan() {
    vkFreeMemory(logicalDevice, stagingImageMemory, nullptr);
    vkDestroyImage(logicalDevice, stagingImage, nullptr);
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
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;
    
    if (vkCreateImage(logicalDevice, &imageInfo, nullptr, &stagingImage) != VK_SUCCESS) {
        std::cerr << "Failed to create image." << std::endl;
        exit(-1);
    }
    
    // Find suitable memory type.
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(logicalDevice, stagingImage, &memoryRequirements);
    
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    uint32_t memoryType;
    unsigned int properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (memoryRequirements.memoryTypeBits & (1u << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties )
            memoryType = i;
    }
    
    // Allocate memory.
    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = memoryType;
    
    if (vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &stagingImageMemory) != VK_SUCCESS) {
        std::cerr << "Failed to allocate image memory." << std::endl;
        exit(-1);
    }
    
    UNIMPLEMENTED
    
    // Clean up.
    delete[] pixels;
    
    return 0;
}

void Texture2DVulkan::bind(unsigned int slot) {
    UNIMPLEMENTED
}
