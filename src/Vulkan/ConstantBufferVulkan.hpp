#pragma once

#include "../Base/ConstantBuffer.hpp"

#include <vulkan/vulkan.h>
#include <map>

class ConstantBufferVulkan : public ConstantBuffer {
    public:
        ConstantBufferVulkan(std::string NAME, unsigned int location, VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool);
        ~ConstantBufferVulkan() final;
        void setData(const void* data, size_t size, Material* m, unsigned int location) final;
        void bind(Material* material) final;
        
        VkDescriptorSet getDescriptorSet() const;
        uint32_t getOffset();
        
    private:
        VkDeviceSize createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);
        void createDescriptorLayout();
        void createDescriptorSet(VkDeviceSize size);
        
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice;
        VkDescriptorPool descriptorPool;
        
        unsigned int location;
        uint32_t paddedSize;
        static std::map<unsigned int, VkDescriptorSetLayout> layoutMap;
        static std::map<unsigned int, VkDescriptorSet> descriptorSetMap;
        static std::map<unsigned int, VkBuffer> bufferMap;
        static std::map<unsigned int, VkDeviceMemory> memoryMap;
        static std::map<unsigned int, unsigned int> offsetMap;
        const void * tempData;
};
