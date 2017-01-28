#include "../OpenGL/OpenGLRenderer.hpp"
#include "Renderer.hpp"


Renderer* Renderer::makeRenderer(BACKEND option)
{
	if (option == BACKEND::GL45)
		return new OpenGLRenderer();
    
    return nullptr;
}
