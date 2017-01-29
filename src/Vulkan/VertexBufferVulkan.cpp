#include "VertexBufferVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

VertexBufferVulkan::VertexBufferVulkan() {
    UNIMPLEMENTED
}

VertexBufferVulkan::~VertexBufferVulkan() {
    UNIMPLEMENTED
}

void VertexBufferVulkan::setData(const void* data, size_t size, DATA_USAGE usage) {
    UNIMPLEMENTED
}

void VertexBufferVulkan::bind(size_t offset, size_t size, unsigned int location) {
    UNIMPLEMENTED
}

void VertexBufferVulkan::unbind() {
    UNIMPLEMENTED
}

size_t VertexBufferVulkan::getSize() {
    UNIMPLEMENTED
    return 0;
}
