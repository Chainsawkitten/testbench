#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>
#include "../Base/Renderer.hpp"

class VulkanRenderer : public Renderer {
    public:
        VulkanRenderer();
        ~VulkanRenderer();
        
        Material* makeMaterial();
        Mesh* makeMesh();
        VertexBuffer* makeVertexBuffer();
        ConstantBuffer* makeConstantBuffer(std::string name, unsigned int location);
        RenderState* makeRenderState();
        Texture2D* makeTexture2D();
        Sampler2D* makeSampler2D();
        std::string getShaderPath();
        std::string getShaderExtension();
        
        int initialize(unsigned int width = 640, unsigned int height = 480);
        int shutdown();
        
        void setClearColor(float, float, float, float);
        void clearBuffer(unsigned int);
        void setRenderState(RenderState* ps);
        void submit(Mesh* mesh);
        void frame();
        void present();
        
    private:
        SDL_Window* window;
        
        VkInstance instance;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};
