#pragma once

#include "../Base/ConstantBuffer.hpp"

#include <vulkan/vulkan.h>

class ConstantBufferVulkan : public ConstantBuffer {
    public:
        ConstantBufferVulkan(std::string NAME, unsigned int location, VkDevice* logicalDevice, VkPhysicalDevice* physicalDevice);
        ~ConstantBufferVulkan() final;
        void setData(const void* data, size_t size, Material* m, unsigned int location) final;
        void bind(Material* material) final;
    private:
        unsigned int location;
        VkDevice* logicalDevice;
        VkPhysicalDevice* physicalDevice;
        VkBuffer storageBuffer;
        VkDeviceMemory uniformBufferMemory;
        VkDescriptorSetLayout descriptorSetLayout;
};
