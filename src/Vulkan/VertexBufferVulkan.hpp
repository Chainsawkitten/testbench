#pragma once

#include "../Base/VertexBuffer.hpp"
#include <vulkan/vulkan.h>
#include <map>

class VertexBufferVulkan : public VertexBuffer {
    public:
        VertexBufferVulkan(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool) ;
        ~VertexBufferVulkan() final;
        
        void setData(const void* data, size_t size, DATA_USAGE usage) final;
        void bind(size_t offset, size_t size, unsigned int location) final;
        void unbind() final;
        size_t getSize() final;
        
    private:
        VkDeviceSize createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);
        void createDescriptorLayout(uint32_t location);
        void createDescriptorSet(uint32_t location);
        
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice;
        VkDescriptorPool descriptorPool;
        
        const void* tempData;
        static std::map<unsigned int, unsigned int> offsetMap;
        static std::map<unsigned int, VkDeviceMemory> memoryMap;
        static std::map<unsigned int, VkBuffer> bufferMap;
        static std::map<unsigned int, VkDescriptorSetLayout> layoutMap;
        static std::map<unsigned int, VkDescriptorSet> descriptorSetMap;
};
