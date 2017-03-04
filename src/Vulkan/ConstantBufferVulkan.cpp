#include "ConstantBufferVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

ConstantBufferVulkan::ConstantBufferVulkan(std::string NAME, unsigned int location, VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice) {
    this->location = location;
    this->physicalDevice = physicalDevice;
    this->logicalDevice = logicalDevice;
}

ConstantBufferVulkan::~ConstantBufferVulkan() {
    
}

void ConstantBufferVulkan::setData(const void* data, size_t size, Material* m, unsigned int location) {
    // Create vertex storage buffer.
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
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

    //Allocate memory on the device
    if(vkAllocateMemory(*logicalDevice, &allocationInfo, nullptr, &uniformBufferMemory) != VK_SUCCESS)
        std::cerr << "Could not allocate memory!" << std::endl;

    //Copy data from data to mapped memory.
    void* mappedMemory;
    vkMapMemory(*logicalDevice, uniformBufferMemory, 0, bufferInfo.size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(*logicalDevice, uniformBufferMemory);

    vkBindBufferMemory(*logicalDevice, storageBuffer, uniformBufferMemory, 0);
}

void ConstantBufferVulkan::bind(Material* material) {
    VkDescriptorSetLayoutBinding vertexLayoutBinding = {};
    vertexLayoutBinding.binding = location;
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
