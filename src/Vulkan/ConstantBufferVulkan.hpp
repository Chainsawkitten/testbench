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
        
    private:
        void createDescriptorLayout();
        
        VkDevice logicalDevice;
        VkPhysicalDevice physicalDevice;
        VkDescriptorPool descriptorPool;
        
        unsigned int location;
        static std::map<unsigned int, VkDescriptorSetLayout> layoutMap;
        static std::map<unsigned int, VkBuffer> bufferMap;
        static std::map<unsigned int, VkDeviceMemory> memoryMap;
        static std::map<unsigned int, unsigned int> offsetMap;
        const void * tempData;
};
