#include "VertexBufferVulkan.hpp"

#include <iostream>

std::map<unsigned int, unsigned int> VertexBufferVulkan::offsetMap;
std::map<unsigned int, VkDeviceMemory> VertexBufferVulkan::memoryMap;
std::map<unsigned int, VkBuffer> VertexBufferVulkan::bufferMap;
std::map<unsigned int, VkDescriptorSetLayout> VertexBufferVulkan::layoutMap;

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

VertexBufferVulkan::VertexBufferVulkan(VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice) {
    this->physicalDevice = physicalDevice;
    this->logicalDevice = logicalDevice;
}

VertexBufferVulkan::~VertexBufferVulkan() {
    vkDestroyBuffer(*logicalDevice, storageBuffer, nullptr);
    vkDestroyDescriptorSetLayout(*logicalDevice, descriptorSetLayout, nullptr);
    vkFreeMemory(*logicalDevice, storageBufferObjectMemory, nullptr);
}

void VertexBufferVulkan::setData(const void* data, size_t size, DATA_USAGE usage) {

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

    totalSize = size;
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
    if(vkAllocateMemory(*logicalDevice, &allocationInfo, nullptr, &storageBufferObjectMemory) != VK_SUCCESS)
        std::cerr << "Could not allocate memory!" << std::endl;

    //Copy data from data to mapped memory.
    void* mappedMemory;
    vkMapMemory(*logicalDevice, storageBufferObjectMemory, 0, bufferInfo.size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(*logicalDevice, storageBufferObjectMemory);

    vkBindBufferMemory(*logicalDevice, storageBuffer, storageBufferObjectMemory, 0);
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location) {
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

void VertexBufferVulkan::unbind() {
    // Function intentionally left unimplemented.
}

size_t VertexBufferVulkan::getSize() {
    return totalSize;
}
