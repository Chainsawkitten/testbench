#include "ConstantBufferVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
    std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
    }

std::map<unsigned int, unsigned int> ConstantBufferVulkan::offsetMap;
std::map<unsigned int, VkDeviceMemory> ConstantBufferVulkan::memoryMap;
std::map<unsigned int, VkBuffer> ConstantBufferVulkan::bufferMap;
std::map<unsigned int, VkDescriptorSetLayout> ConstantBufferVulkan::layoutMap;

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
    // Create uniform buffer.
    if (offsetMap.find(location) == offsetMap.end() ) {
        VkBufferCreateInfo bufferInfo = {};
        VkBuffer newBuffer;
        VkDeviceMemory newMemory;
        VkDescriptorSetLayout newLayout;
        
        offsetMap.insert(std::make_pair(location, 0));
        memoryMap.insert(std::make_pair(location, newMemory));
        bufferMap.insert(std::make_pair(location, newBuffer));
        layoutMap.insert(std::make_pair(location, newLayout));
        
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size*20000;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        
        if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &bufferMap[location]) != VK_SUCCESS)
            std::cerr << "Could not create uniform buffer!" << std::endl;
        
        // Get information about device memory.
        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(logicalDevice, bufferMap[location], &memoryRequirements);
        
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
        
        // Find suitable memory type.
        uint32_t memoryType;
        int properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if (memoryRequirements.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties )
                memoryType = i;
        }
        
        VkMemoryAllocateInfo allocationInfo = {};
        allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocationInfo.allocationSize = memoryRequirements.size;
        allocationInfo.memoryTypeIndex = memoryType;
        
        // Allocate memory on the device
        if (vkAllocateMemory(logicalDevice, &allocationInfo, nullptr, &memoryMap[location]) != VK_SUCCESS)
            std::cerr << "Could not allocate memory!" << std::endl;
        
        // Bind memory to buffer.
        vkBindBufferMemory(logicalDevice, bufferMap[location], memoryMap[location], 0);
        
        // Create descriptor set layout.
        createDescriptorLayout();
        
        /// @todo Create descriptor set.
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

void ConstantBufferVulkan::createDescriptorLayout() {
    VkDescriptorSetLayoutBinding uniformLayoutBinding = {};
    uniformLayoutBinding.binding = location;
    uniformLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniformLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uniformLayoutBinding.pImmutableSamplers = nullptr;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uniformLayoutBinding;
    
    if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &layoutMap[location]))
        std::cerr << "Could not create descriptor set!" << std::endl;
}
