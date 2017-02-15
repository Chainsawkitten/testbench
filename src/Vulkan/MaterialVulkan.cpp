#include "MaterialVulkan.hpp"

#include <iostream>
#include <fstream>

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
    // Compile shaders.
    if (compileShader(ShaderType::VS, errString) < 0) {
        std::cout << errString << std::endl;
        return -1;
    }
    
    if (compileShader(ShaderType::PS, errString) < 0) {
        std::cout << errString << std::endl;
        return -1;
    }
    
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

int MaterialVulkan::compileShader(ShaderType type, std::string& errString) {
    // Read shader file into string.
    std::ifstream shaderFile(shaderFileNames[type]);
    std::string shaderText;
    if (shaderFile.is_open()) {
        shaderText = std::string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
        shaderFile.close();
    } else {
        errString = "Cannot find file: " + shaderFileNames[type];
        return -1;
    }
    
    // Add defines.
    std::string outShaderText = "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";
    for (const std::string& define : shaderDefines[type])
        outShaderText += define;
    outShaderText += shaderText;
    
    // Output to temp file.
    std::ofstream outShaderFile("temp.glsl");
    outShaderFile << outShaderText;
    outShaderFile.close();
    
    UNIMPLEMENTED
    
    return -1;
}
