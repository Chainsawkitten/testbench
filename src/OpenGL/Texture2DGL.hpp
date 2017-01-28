#pragma once

#include <GL/glew.h>

#include "../Base/Texture2D.hpp"
#include "Sampler2DGL.hpp"

class Texture2DGL : public Texture2D {
    public:
        Texture2DGL();
        ~Texture2DGL() final;
        
        int loadFromFile(std::string filename);
        void bind(unsigned int slot);
        
        GLuint textureHandle = 0;
};

