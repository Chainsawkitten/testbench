#include "MaterialVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

MaterialVulkan::MaterialVulkan() {
    UNIMPLEMENTED
}

MaterialVulkan::~MaterialVulkan() {
    UNIMPLEMENTED
}

void MaterialVulkan::setShader(const std::string& shaderFileName, ShaderType type) {
    if (shaderFileNames.find(type) != shaderFileNames.end())
        removeShader(type);
    
    shaderFileNames[type] = shaderFileName;
}

void MaterialVulkan::removeShader(ShaderType type) {
    UNIMPLEMENTED
}

int MaterialVulkan::compileMaterial(std::string& errString) {
    UNIMPLEMENTED
    return 0;
}

int MaterialVulkan::enable() {
    UNIMPLEMENTED
    return 0;
}

void MaterialVulkan::disable() {
    UNIMPLEMENTED
}

void MaterialVulkan::setDiffuse(Color c) {
    UNIMPLEMENTED
}

void MaterialVulkan::updateConstantBuffer(const void* data, size_t size, unsigned int location) {
    UNIMPLEMENTED
}

void MaterialVulkan::addConstantBuffer(std::string name, unsigned int location) {
    UNIMPLEMENTED
}
