#include "VertexBufferVulkan.hpp"

#include <iostream>

std::map<unsigned int, unsigned int> VertexBufferVulkan::offsetMap;
std::map<unsigned int, VkDeviceMemory> VertexBufferVulkan::memoryMap;
std::map<unsigned int, VkBuffer> VertexBufferVulkan::bufferMap;
std::map<unsigned int, VkDescriptorSetLayout> VertexBufferVulkan::layoutMap;
std::map<unsigned int, VkDescriptorSet> VertexBufferVulkan::descriptorSetMap;

#define UNIMPLEMENTED {\
    std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
    }

VertexBufferVulkan::VertexBufferVulkan(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool) {
    this->physicalDevice = physicalDevice;
    this->logicalDevice = logicalDevice;
    this->descriptorPool = descriptorPool;
}

VertexBufferVulkan::~VertexBufferVulkan() {
    for (auto& it : bufferMap)
        vkDestroyBuffer(logicalDevice, it.second, nullptr);
    bufferMap.clear();
    
    for (auto& it : layoutMap)
        vkDestroyDescriptorSetLayout(logicalDevice, it.second, nullptr);
    layoutMap.clear();
    
    for (auto& it : memoryMap)
        vkFreeMemory(logicalDevice, it.second, nullptr);
    memoryMap.clear();
}

void VertexBufferVulkan::setData(const void* data, size_t size, DATA_USAGE usage) {
    tempData = data;
    // @todo: Find static/dynamic/et.c equivalents in vulkan.
    // VkBufferUsageFlagBits vulkanUsage;
    /*switch(DATA_USAGE) {
        case STATIC:
            break;
        case DYNAMIC:
            vulkanUsage = vulkanUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            break;
        case DONTCARE:
            break;
        default:
            break;
    }*/
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location) {
    // Incredibly crude and dodgy workaround incoming. Thanks Fransisco...
    if (offsetMap.find(location) == offsetMap.end() ) {
        offsetMap[location] = 0;
        
        // Create storage buffer.
        createBuffer(size * 2000, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &bufferMap[location], &memoryMap[location]);
        
        // Create descriptor set layout.
        createDescriptorLayout(location);
        
        // Create descriptor set.
        createDescriptorSet(location, size * 2000);
    } else
        offsetMap[location]++;
    
    // Copy data from data to mapped memory.
    void* mappedMemory;
    vkMapMemory(logicalDevice, memoryMap[location], offsetMap[location]*size, size, 0, &mappedMemory);
    memcpy(mappedMemory, tempData, size);
    vkUnmapMemory(logicalDevice, memoryMap[location]);
}

void VertexBufferVulkan::unbind() {
    // Function intentionally left unimplemented.
}

size_t VertexBufferVulkan::getSize() {
    return size_t(2000*3*3);
}

VkDeviceSize VertexBufferVulkan::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
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

void VertexBufferVulkan::createDescriptorLayout(uint32_t location) {
    VkDescriptorSetLayoutBinding vertexLayoutBinding = {};
    vertexLayoutBinding.binding = location;
    vertexLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    vertexLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    vertexLayoutBinding.pImmutableSamplers = nullptr;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &vertexLayoutBinding;
    
    if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layoutMap[location]))
        std::cerr << "Could not create descriptor set!" << std::endl;
}

void VertexBufferVulkan::createDescriptorSet(uint32_t location, VkDeviceSize size) {
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
}
