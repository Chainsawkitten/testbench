#include "VulkanRenderer.hpp"

#include <iostream>
#include <vulkan/vulkan.h>
#include "ConstantBufferVulkan.hpp"
#include "MaterialVulkan.hpp"
#include "MeshVulkan.hpp"
#include "RenderStateVulkan.hpp"
#include "Sampler2DVulkan.hpp"
#include "Texture2DVulkan.hpp"
#include "VertexBufferVulkan.hpp"

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

VulkanRenderer::VulkanRenderer() {
    
}

VulkanRenderer::~VulkanRenderer() {
    
}

Mesh* VulkanRenderer::makeMesh() { 
    return new MeshVulkan();
}

Texture2D* VulkanRenderer::makeTexture2D() {
    return new Texture2DVulkan();
}

Sampler2D* VulkanRenderer::makeSampler2D() {
    return new Sampler2DVulkan();
}

ConstantBuffer* VulkanRenderer::makeConstantBuffer(std::string name, unsigned int location) { 
    return new ConstantBufferVulkan(name, location);
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
    return new VertexBufferVulkan();
}

Material* VulkanRenderer::makeMaterial() { 
    return new MaterialVulkan();
}

RenderState* VulkanRenderer::makeRenderState() { 
    return new RenderStateVulkan();
}

int VulkanRenderer::initialize(unsigned int width, unsigned int height) {
    // Init SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "%s", SDL_GetError());
        exit(-1);
    }
    
    // Create window.
    window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    
    // TODO: Init Vulkan.
    
    UNIMPLEMENTED
    return -1;
}

int VulkanRenderer::shutdown() {
    UNIMPLEMENTED
    return -1;
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

void VulkanRenderer::submit(Mesh* mesh) {
    UNIMPLEMENTED
}

void VulkanRenderer::frame() {
    UNIMPLEMENTED
}

void VulkanRenderer::present() {
    UNIMPLEMENTED
}
