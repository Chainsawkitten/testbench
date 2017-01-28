#pragma once

#include <vector>
#include "../Base/RenderState.hpp"

class RenderStateGL : public RenderState {
    public:
        RenderStateGL();
        ~RenderStateGL() final;
        void setWireFrame(bool);
        void set();
        
        void setGlobalWireFrame(bool* global);
        
    private:
        bool _wireframe;
        bool* globalWireFrame;
};

