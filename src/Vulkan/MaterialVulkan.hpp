#pragma once

#include "../Base/Material.hpp"

class MaterialVulkan : public Material {
    public:
        MaterialVulkan();
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
        
    private:
        int compileShader(ShaderType type, std::string& errString);
        std::string readFile(const std::string& filename);
        
        std::string shaderNames[4];
        std::map<ShaderType, std::string> shaderExtensions;
};
