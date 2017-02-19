#include "MaterialVulkan.hpp"

#include <iostream>
#include <fstream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

MaterialVulkan::MaterialVulkan(VkDevice device) {
    this->device = device;
    shaderExtensions[ShaderType::VS] = "vert";
    shaderExtensions[ShaderType::GS] = "geom";
    shaderExtensions[ShaderType::PS] = "frag";
    shaderExtensions[ShaderType::CS] = "comp";
}

MaterialVulkan::~MaterialVulkan() {
    // Clean up shader modules.
    for (auto& it : shaderModules)
        vkDestroyShaderModule(device, it.second, nullptr);
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
    std::string shaderText = readFile(shaderFileNames[type]);
    
    // Add defines.
    std::string outShaderText = "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";
    for (const std::string& define : shaderDefines[type])
        outShaderText += define;
    outShaderText += shaderText;
    
    // Output to temp file.
    std::ofstream outShaderFile("temp." + shaderExtensions[type]);
    outShaderFile << outShaderText;
    outShaderFile.close();
    
    // Compile to SPIR-V.
    /// @todo Don't use a system call. Seriously...
    system(("glslangValidator.exe -V temp." + shaderExtensions[type]).c_str());
    
    // Read binary shader.
    std::vector<char> binaryShader = readFile2(shaderExtensions[type] + ".spv");
    
    // Create shader module.
    createShaderModule(type, binaryShader);
    
    UNIMPLEMENTED
    
    return -1;
}

void MaterialVulkan::createShaderModule(ShaderType type, const std::vector<char>& source) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = (uint32_t*) source.data();
    
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModules[type]) != VK_SUCCESS) {
        std::cerr << "Failed to create shader module." << std::endl;
        exit(-1);
    }
}

std::string MaterialVulkan::readFile(const std::string& filename) {
    std::ifstream shaderFile(filename);
    std::string text;
    if (shaderFile.is_open()) {
        text = std::string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
        shaderFile.close();
    } else {
        std::cerr << "Cannot find file: " << filename << std::endl;
        exit(-1);
    }
    
    return text;
}

std::vector<char> MaterialVulkan::readFile2(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Cannot find file: " << filename << std::endl;
        exit(-1);
    }
    
    std::size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    
    return buffer;
}
