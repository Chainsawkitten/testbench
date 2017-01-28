#include "Texture2DVulkan.hpp"

#include <iostream>

#define UNIMPLEMENTED {\
std::cout << "Unimplemented method in: " << __FILE__ << ":" << __LINE__ << std::endl;\
}

Texture2DVulkan::Texture2DVulkan() {
    
}

Texture2DVulkan::~Texture2DVulkan() {
    
}

int Texture2DVulkan::loadFromFile(std::string filename) {
    UNIMPLEMENTED
    return 0;
}

void Texture2DVulkan::bind(unsigned int slot) {
    UNIMPLEMENTED
}
