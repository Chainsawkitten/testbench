#include "ConstantBufferVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
    std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
    }

std::map<unsigned int, unsigned int> ConstantBufferVulkan::offsetMap;
std::map<unsigned int, VkDeviceMemory> ConstantBufferVulkan::memoryMap;
std::map<unsigned int, VkBuffer> ConstantBufferVulkan::bufferMap;
std::map<unsigned int, VkDescriptorSetLayout> ConstantBufferVulkan::layoutMap;
std::map<unsigned int, VkDescriptorSet> ConstantBufferVulkan::descriptorSetMap;

ConstantBufferVulkan::ConstantBufferVulkan(std::string NAME, unsigned int location, VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool) {
    this->location = location;
    this->physicalDevice = physicalDevice;
    this->logicalDevice = logicalDevice;
    this->descriptorPool = descriptorPool;
}

ConstantBufferVulkan::~ConstantBufferVulkan() {
    // @todo: Remove this, or do it better or whatever.
    //vkDestroyBuffer(*logicalDevice, storageBuffer, nullptr);
    //vkDestroyDescriptorSetLayout(*logicalDevice, descriptorSetLayout, nullptr);
    //vkFreeMemory(*logicalDevice, uniformBufferMemory, nullptr);
}

void ConstantBufferVulkan::setData(const void* data, size_t size, Material* m, unsigned int location) {
    if (offsetMap.find(location) == offsetMap.end() ) {
        offsetMap[location] = 0;
        
        // Create uniform buffer.
        VkDeviceSize deviceSize = createBuffer(size * 2000, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &bufferMap[location], &memoryMap[location]);
        
        // Create descriptor set layout.
        createDescriptorLayout();
        
        //  Create descriptor set.
        createDescriptorSet(deviceSize);
    } else
        offsetMap[location]++;
    
    if (offsetMap[location] == 2000)
        offsetMap[location] = 0;
    
    // Copy data from data to mapped memory.
    void* mappedMemory;
    vkMapMemory(logicalDevice, memoryMap[location], offsetMap[location]*size, size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(logicalDevice, memoryMap[location]);
}

void ConstantBufferVulkan::bind(Material* material) {
    // Intentionally not implemented.
}

VkDeviceSize ConstantBufferVulkan::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
    // Create buffer.
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
        std::cerr << "Failed to create buffer." << std::endl;
        exit(-1);
    }
    
    // Get information about device memory.
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, *buffer, &memoryRequirements);
    
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    // Find suitable memory type.
    uint32_t memoryType;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (memoryRequirements.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties )
            memoryType = i;
    }
    
    // Allocate buffer memory.
    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = memoryType;
    
    if (vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, bufferMemory) != VK_SUCCESS) {
        std::cerr << "Failed to allocate buffer memory." << std::endl;
        exit(-1);
    }
    
    vkBindBufferMemory(logicalDevice, *buffer, *bufferMemory, 0);
    
    return memoryRequirements.size;
}

void ConstantBufferVulkan::createDescriptorLayout() {
    VkDescriptorSetLayoutBinding uniformLayoutBinding = {};
    uniformLayoutBinding.binding = location;
    uniformLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniformLayoutBinding.descriptorCount = 1;
    uniformLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uniformLayoutBinding.pImmutableSamplers = nullptr;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uniformLayoutBinding;
    
    if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layoutMap[location]))
        std::cerr << "Could not create descriptor set!" << std::endl;
}

void ConstantBufferVulkan::createDescriptorSet(VkDeviceSize size) {
    // Allocate descriptor set.
    VkDescriptorSetAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = descriptorPool;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = &layoutMap[location];
    
    if (vkAllocateDescriptorSets(logicalDevice, &allocateInfo, &descriptorSetMap[location]) != VK_SUCCESS) {
        std::cerr << "Failed to allocate descriptor set" << std::endl;
        exit(-1);
    }
    
    // Update descriptor set.
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = bufferMap[location];
    bufferInfo.offset = 0;
    bufferInfo.range = size;
    
    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSetMap[location];
    descriptorWrite.dstBinding = location;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    
    vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);
}
