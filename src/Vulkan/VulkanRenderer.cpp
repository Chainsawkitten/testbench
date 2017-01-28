#include "VulkanRenderer.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

VulkanRenderer::VulkanRenderer() {
    
}

VulkanRenderer::~VulkanRenderer() {
    
}

int VulkanRenderer::shutdown() {
    UNIMPLEMENTED
    return -1;
}

Mesh* VulkanRenderer::makeMesh() { 
    UNIMPLEMENTED
    return nullptr;
}

Texture2D* VulkanRenderer::makeTexture2D() {
    UNIMPLEMENTED
    return nullptr;
}

Sampler2D* VulkanRenderer::makeSampler2D() {
    UNIMPLEMENTED
    return nullptr;
}

ConstantBuffer* VulkanRenderer::makeConstantBuffer(std::string NAME, unsigned int location) { 
    UNIMPLEMENTED
    return nullptr;
}

std::string VulkanRenderer::getShaderPath() {
    UNIMPLEMENTED
    return "";
}

std::string VulkanRenderer::getShaderExtension() {
    UNIMPLEMENTED
    return "";
}

VertexBuffer* VulkanRenderer::makeVertexBuffer() { 
    UNIMPLEMENTED
    return nullptr;
}

Material* VulkanRenderer::makeMaterial() { 
    UNIMPLEMENTED
    return nullptr;
}

RenderState* VulkanRenderer::makeRenderState() { 
    UNIMPLEMENTED
    return nullptr;
}

int VulkanRenderer::initialize(unsigned int width, unsigned int height) {
    UNIMPLEMENTED
    return -1;
}

void VulkanRenderer::submit(Mesh* mesh) {
    UNIMPLEMENTED
}

void VulkanRenderer::frame() {
    UNIMPLEMENTED
}

void VulkanRenderer::present() {
    UNIMPLEMENTED
}

void VulkanRenderer::setClearColor(float r, float g, float b, float a) {
    UNIMPLEMENTED
}

void VulkanRenderer::clearBuffer(unsigned int flag) {
    UNIMPLEMENTED
}

void VulkanRenderer::setRenderState(RenderState* ps) {
    UNIMPLEMENTED
}
