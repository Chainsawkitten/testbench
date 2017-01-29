#pragma once

#include "../Base/VertexBuffer.hpp"

class VertexBufferVulkan : public VertexBuffer {
    public:
        VertexBufferVulkan();
        ~VertexBufferVulkan() final;
        
        void setData(const void* data, size_t size, DATA_USAGE usage) final;
        void bind(size_t offset, size_t size, unsigned int location) final;
        void unbind() final;
        size_t getSize() final;
};
