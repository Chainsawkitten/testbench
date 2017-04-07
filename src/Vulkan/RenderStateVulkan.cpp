#include "RenderStateVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

RenderStateVulkan::RenderStateVulkan() {
    
}

RenderStateVulkan::~RenderStateVulkan() {
}

void RenderStateVulkan::set() {
    UNIMPLEMENTED
}

void RenderStateVulkan::setWireFrame(bool wireframe) {
    // Intentionally left unimplemented.
}
