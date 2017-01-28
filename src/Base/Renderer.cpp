#include "Renderer.hpp"

#include "../OpenGL/OpenGLRenderer.hpp"

Renderer* Renderer::makeRenderer(BACKEND option) {
	if (option == BACKEND::GL45)
		return new OpenGLRenderer();
    
    return nullptr;
}
