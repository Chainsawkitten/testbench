#pragma once

#include <unordered_map>
#include "../IA.h"
#include "VertexBuffer.hpp"
#include "../Technique.hpp"
#include "Transform.hpp"
#include "ConstantBuffer.hpp"
#include "Texture2D.hpp"

class Mesh {
    public:
        Mesh();
        ~Mesh();
        
        // technique has: Material, RenderState, Attachments (color, depth, etc)
        Technique* technique; 
        
        // translation buffers
        ConstantBuffer* txBuffer;
        // local copy of the translation
        Transform* transform;
        
        struct VertexBufferBind {
                size_t numElements, offset;
                VertexBuffer* buffer;
        };
        
        void addTexture(Texture2D* texture, unsigned int slot);
        
        // array of buffers with locations (binding points in shaders)
        void addIAVertexBufferBinding(VertexBuffer* buffer, size_t offset, size_t size, unsigned int inputStream);
        void bindIAVertexBuffer(unsigned int location);
        std::unordered_map<unsigned int, VertexBufferBind> geometryBuffers;
        std::unordered_map<unsigned int, Texture2D*> textures;
};
