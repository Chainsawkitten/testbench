#pragma once

#include "../Base/Material.hpp"
#include <vector>
#include <vulkan/vulkan.h>

class MaterialVulkan : public Material {
    public:
        MaterialVulkan(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorPool descriptorPool);
        ~MaterialVulkan();
        
        void setShader(const std::string& shaderFileName, ShaderType type) final;
        void removeShader(ShaderType type) final;
        int compileMaterial(std::string& errString) final;
        int enable() final;
        void disable() final;
        void setDiffuse(Color c) final;
        
        // location identifies the constant buffer in a unique way
        void updateConstantBuffer(const void* data, size_t size, unsigned int location) final;
        
        // slower version using a string
        void addConstantBuffer(std::string name, unsigned int location) final;
        
        VkPipeline getPipeline() const;
        VkPipelineLayout getPipelineLayout() const;
        
        VkDescriptorSet getDiffuseDescriptorSet() const;
        
    private:
        int compileShader(ShaderType type, std::string& errString);
        void createShaderModule(ShaderType type, const std::vector<char>& source);
        VkPipelineShaderStageCreateInfo createShaderStage(ShaderType type);
        static std::string readFile(const std::string& filename);
        static std::vector<char> readFile2(const std::string& filename);
        void createDescriptorSetLayouts();
        void createUniformDescriptorSet(VkDescriptorSetLayout layout);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);
        
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkDescriptorPool descriptorPool;
        std::map<ShaderType, std::string> shaderExtensions;
        std::map<ShaderType, VkShaderModule> shaderModules;
        std::map<ShaderType, VkShaderStageFlagBits> shaderStageFlags;
        VkExtent2D swapChainExtent;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        
        VkBuffer diffuseBuffer;
        VkDeviceMemory diffuseBufferMemory;
        VkDescriptorSet diffuseDescriptorSet;
        
        static int materialIndex;
};
