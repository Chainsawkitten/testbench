#pragma once

#include "../Base/Renderer.hpp"

#include <SDL.h>
#include <GL/glew.h>

class OpenGLRenderer : public Renderer {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer();
        
        Material* makeMaterial();
        Mesh* makeMesh();
        VertexBuffer* makeVertexBuffer();
        ConstantBuffer* makeConstantBuffer(std::string NAME, unsigned int location);
        RenderState* makeRenderState();
        Texture2D* makeTexture2D();
        Sampler2D* makeSampler2D();
        std::string getShaderPath();
        std::string getShaderExtension();
        
        int initialize(unsigned int width = 640, unsigned int height = 480);
        int shutdown();
        
        void setClearColor(float, float, float, float);
        void clearBuffer(unsigned int);
        void setRenderState(RenderState* ps);
        void submit(Mesh* mesh);
        void frame();
        void present();
        
    private:
        SDL_Window* window;
        SDL_GLContext context;
        
        std::vector<Mesh*> drawList;
        
        bool globalWireframeMode = false;
        
        //int initializeOpenGL(int major, int minor, unsigned int width, unsigned int height);
        float clearColor[4] = { 0,0,0,0 };
        std::unordered_map<int, int> BUFFER_MAP = { 
            {0, 0},
            {CLEAR_BUFFER_FLAGS::COLOR, GL_COLOR_BUFFER_BIT },
            {CLEAR_BUFFER_FLAGS::DEPTH, GL_DEPTH_BUFFER_BIT }, 
            {CLEAR_BUFFER_FLAGS::STENCIL, GL_STENCIL_BUFFER_BIT }
        };
};
