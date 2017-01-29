#pragma once

#include "../Base/RenderState.hpp"

class RenderStateVulkan : public RenderState {
    public:
        RenderStateVulkan();
        ~RenderStateVulkan() final;
        void setWireFrame(bool wireframe) final;
        void set() final;
};
