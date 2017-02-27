#pragma once

#include "../Base/VertexBuffer.hpp"
#include "vulkan/vulkan.h"

class VertexBufferVulkan : public VertexBuffer {
    public:
        VertexBufferVulkan(VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice) ;
        ~VertexBufferVulkan() final;
        
        void setData(const void* data, size_t size, DATA_USAGE usage) final;
        void bind(size_t offset, size_t size, unsigned int location) final;
        void unbind() final;
        size_t getSize() final;
    private:
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkVertexInputAttributeDescription attributeDescription = {};
        VkVertexInputBindingDescription bindingDescription = {};
        VkDevice* logicalDevice;
        VkPhysicalDevice* physicalDevice;
        size_t totalSize;
};
