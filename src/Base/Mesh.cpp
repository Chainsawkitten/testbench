#include "Mesh.hpp"

Mesh::Mesh() {
    
}

Mesh::~Mesh() {
    
}

/*
    buffer: is a VertexBuffer*
    offset: offset of the first byte in the buffer used when binding
    size: how many elements (how many points, normals, UVs) should be read from this buffer
    inputStream: location of the binding in the VertexShader
*/
void Mesh::addIAVertexBufferBinding(VertexBuffer* buffer, size_t offset, size_t numElements, unsigned int inputStream) {
    // inputStream is unique (has to be!) for this Mesh
    geometryBuffers[inputStream] = { numElements, offset, buffer };
}

void Mesh::bindIAVertexBuffer(unsigned int location) {
    // no checking if the key is valid...TODO
    const VertexBufferBind& vb = geometryBuffers[location];
    vb.buffer->bind(vb.offset,vb.buffer->getSize(),location);
}

// note, slot is a value set in the shader as well (registry, or binding)
void Mesh::addTexture(Texture2D* texture, unsigned int slot) {
    // would override the slot if there is another pointer here.
    textures[slot] = texture;
}
