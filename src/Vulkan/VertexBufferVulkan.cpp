#include "VertexBufferVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

VertexBufferVulkan::VertexBufferVulkan(VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice) {
    this->physicalDevice = physicalDevice;
    this->logicalDevice = logicalDevice;
}

VertexBufferVulkan::~VertexBufferVulkan() {
    //UNIMPLEMENTED
}

void VertexBufferVulkan::setData(const void* data, size_t size, DATA_USAGE usage) {
    totalSize = size;
    // Create vertex buffer.
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(*logicalDevice, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
        std::cerr << "Could not create vertex buffer!" << std::endl;

    // Get information about device memory.
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(*logicalDevice, vertexBuffer, &memoryRequirements);

    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(*physicalDevice, &memoryProperties);

    uint32_t memoryType;

    // Find suitable memory type.
    int properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if(memoryRequirements.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties )
            memoryType = i;
    }

    VkMemoryAllocateInfo allocationInfo = {};
    allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocationInfo.allocationSize = memoryRequirements.size;
    allocationInfo.memoryTypeIndex = memoryType;

    if(vkAllocateMemory(*logicalDevice, &allocationInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
        std::cerr << "Could not allocate memory!" << std::endl;

    vkBindBufferMemory(*logicalDevice, vertexBuffer, vertexBufferMemory, 0);

    void* mappedMemory;
    vkMapMemory(*logicalDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(*logicalDevice, vertexBufferMemory);
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location) {
    VkFormat format = VK_FORMAT_UNDEFINED;

    // Assuming that each element in vertex is 32b float.
    switch(size){
        case 1:
            format = VK_FORMAT_R32_SFLOAT;
            break;

        case 2:
            format = VK_FORMAT_R32G32_SFLOAT;
            break;

        case 3:
            format = VK_FORMAT_R32G32B32_SFLOAT;
            break;

        case 4:
            format = VK_FORMAT_R32G32B32A32_SFLOAT;
            break;
        default:
            std::cerr << "Could not find proper format!" << std::endl;
            break;
    }

    bindingDescription.binding = 0;
    bindingDescription.stride = size;
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    attributeDescription.binding = 0;
    attributeDescription.location = location;
    attributeDescription.format = format;
    attributeDescription.offset = offset;
}

void VertexBufferVulkan::unbind() {
    UNIMPLEMENTED
}

size_t VertexBufferVulkan::getSize() {
    return totalSize;
}
