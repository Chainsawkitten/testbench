#pragma once

#include "../Base/Sampler2D.hpp"
#include <vulkan/vulkan.h>

class Sampler2DVulkan : public Sampler2D {
    public:
        Sampler2DVulkan(VkDevice device);
        ~Sampler2DVulkan() final;
        void setMagFilter(FILTER filter) final;
        void setMinFilter(FILTER filter) final;
        void setWrap(WRAPPING s, WRAPPING t) final;
        
    private:
        VkDevice device;
        VkSampler textureSampler;
};
