#pragma once

#include <GL/glew.h>
#include "../Base/Sampler2D.hpp"

class Sampler2DGL : public Sampler2D {
    public:
        Sampler2DGL();
        virtual ~Sampler2DGL();
        void setMagFilter(FILTER filter);
        void setMinFilter(FILTER filter);
        void setWrap(WRAPPING s, WRAPPING t);
        
        GLuint magFilter, minFilter, wrapS, wrapT;
        GLuint samplerHandler = 0;
};

