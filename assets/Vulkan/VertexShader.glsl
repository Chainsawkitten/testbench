// buffer inputs
/*#ifdef NORMAL
    layout(binding=NORMAL) buffer nor { vec4 normal_in[]; };
    layout(location=NORMAL) out vec4 normal_out;
#endif

#ifdef TEXTCOORD
    layout(binding=TEXTCOORD) buffer text { vec2 uv_in[]; };
    layout(location=TEXTCOORD) out vec2 uv_out;
#endif*/
layout(set=0, binding=POSITION) buffer pos { vec4 position_in[]; };

layout(set=1, binding=TRANSLATION) uniform TRANSLATION_NAME
{
    vec4 translate;
};

void main() {
    /*#ifdef NORMAL
        normal_out = normal_in[gl_VertexIndex];
    #endif
    
    #ifdef TEXTCOORD
        uv_out = uv_in[gl_VertexIndex];
    #endif*/
    
    gl_Position = position_in[gl_VertexIndex] + translate;
    
    // OpenGL and Vulkan use different coordinate systems.
    gl_Position.y = -gl_Position.y;
    gl_Position.z = -gl_Position.z;
}
