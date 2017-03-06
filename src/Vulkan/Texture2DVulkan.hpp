#pragma once

#include "../Base/Texture2D.hpp"
#include <vulkan/vulkan.h>

class Texture2DVulkan : public Texture2D {
    public:
        Texture2DVulkan(VkDevice device);
        ~Texture2DVulkan() final;
        
        int loadFromFile(std::string filename) final;
        void bind(unsigned int slot) final;
        
    private:
        VkDevice device;
        VkImage stagingImage;
        VkDeviceMemory stagingImageMemory;
};
