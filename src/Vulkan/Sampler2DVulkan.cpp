#include "Sampler2DVulkan.hpp"

#include <iostream>

Sampler2DVulkan::Sampler2DVulkan(VkDevice device) {
    this->device = device;
    
    // Hardcode some default values.
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    
    if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        std::cerr << "Failed to create texture sampler." << std::endl;
        exit(-1);
    }
}

Sampler2DVulkan::~Sampler2DVulkan() {
    vkDestroySampler(device, textureSampler, nullptr);
}

void Sampler2DVulkan::setMagFilter(FILTER filter) {
    // Intentionally left unimplemented.
}

void Sampler2DVulkan::setMinFilter(FILTER filter) {
    // Intentionally left unimplemented.
}

void Sampler2DVulkan::setWrap(WRAPPING s, WRAPPING t) {
    // Intentionally left unimplemented.
}

VkSampler Sampler2DVulkan::getTextureSampler() const {
    return textureSampler;
}
