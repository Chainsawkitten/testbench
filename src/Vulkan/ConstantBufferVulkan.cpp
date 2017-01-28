#include "ConstantBufferVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

ConstantBufferVulkan::ConstantBufferVulkan(std::string NAME, unsigned int location) {
    UNIMPLEMENTED
}

ConstantBufferVulkan::~ConstantBufferVulkan() {
    
}

void ConstantBufferVulkan::setData(const void* data, size_t size, Material* m, unsigned int location) {
    UNIMPLEMENTED
}

void ConstantBufferVulkan::bind(Material* material) {
    UNIMPLEMENTED
}
