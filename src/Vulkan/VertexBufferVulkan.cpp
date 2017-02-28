#include "VertexBufferVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

VertexBufferVulkan::VertexBufferVulkan(VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice) {
    this->physicalDevice = physicalDevice;
    this->logicalDevice = logicalDevice;

    VkDescriptorSetLayoutBinding vertexLayoutBinding = {};
    vertexLayoutBinding.binding = 0;
    vertexLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    vertexLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    vertexLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &vertexLayoutBinding;

    if(vkCreateDescriptorSetLayout(*logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout))
        std::cerr << "Could not create descriptor set!" << std::endl;
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

    if(vkCreateBuffer(*logicalDevice, &bufferInfo, nullptr, &storageBuffer) != VK_SUCCESS)
        std::cerr << "Could not create vertex buffer!" << std::endl;

    // Get information about device memory.
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(*logicalDevice, storageBuffer, &memoryRequirements);

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

    if(vkAllocateMemory(*logicalDevice, &allocationInfo, nullptr, &storageBufferObjectMemory) != VK_SUCCESS)
        std::cerr << "Could not allocate memory!" << std::endl;

    vkBindBufferMemory(*logicalDevice, storageBuffer, storageBufferObjectMemory, 0);

    void* mappedMemory;
    vkMapMemory(*logicalDevice, storageBufferObjectMemory, 0, bufferInfo.size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(*logicalDevice, storageBufferObjectMemory);
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location) {

}

void VertexBufferVulkan::unbind() {
    UNIMPLEMENTED
}

size_t VertexBufferVulkan::getSize() {
    return totalSize;
}
