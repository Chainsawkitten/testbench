#include "Renderer.hpp"

#include "../OpenGL/OpenGLRenderer.hpp"
#include "../Vulkan/VulkanRenderer.hpp"

Renderer* Renderer::makeRenderer(BACKEND option) {
	if (option == BACKEND::GL45)
		return new OpenGLRenderer();
    
    if (option == BACKEND::VULKAN)
        return new VulkanRenderer();
    
    return nullptr;
}
