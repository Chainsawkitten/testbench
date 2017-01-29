#pragma once

#include "../Base/Sampler2D.hpp"

class Sampler2DVulkan : public Sampler2D {
    public:
        Sampler2DVulkan();
        ~Sampler2DVulkan() final;
        void setMagFilter(FILTER filter) final;
        void setMinFilter(FILTER filter) final;
        void setWrap(WRAPPING s, WRAPPING t) final;
};
