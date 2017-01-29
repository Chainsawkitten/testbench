#pragma once

#include "../Base/Texture2D.hpp"

class Texture2DVulkan : public Texture2D {
    public:
        Texture2DVulkan();
        ~Texture2DVulkan() final;
        
        int loadFromFile(std::string filename) final;
        void bind(unsigned int slot) final;
};
