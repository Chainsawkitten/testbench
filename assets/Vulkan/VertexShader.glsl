// buffer inputs
/*#ifdef NORMAL
    layout(binding=NORMAL) buffer nor { vec4 normal_in[]; };
    layout(location=NORMAL) out vec4 normal_out;
#endif

#ifdef TEXTCOORD
    layout(binding=TEXTCOORD) buffer text { vec2 uv_in[]; };
    layout(location=TEXTCOORD) out vec2 uv_out;
#endif
layout(binding=POSITION) buffer pos { vec4 position_in[]; };

layout(binding=TRANSLATION) uniform TRANSLATION_NAME
{
    vec4 translate;
};

layout(binding=DIFFUSE_TINT) uniform DIFFUSE_TINT_NAME
{
    vec4 diffuseTint;
};*/

void main() {
    vec4 positions[3];
    positions[0] = vec4(0.0,  0.05, 0.0, 1.0);
    positions[1] = vec4(0.05, -0.05, 0.0, 1.0);
    positions[2] = vec4(-0.05, -0.05, 0.0, 1.0);
    
    /*#ifdef NORMAL
        normal_out = normal_in[gl_VertexIndex];
    #endif
    
    #ifdef TEXTCOORD
        uv_out = uv_in[gl_VertexIndex];
    #endif
    
    gl_Position = position_in[gl_VertexIndex] + translate;*/
    
    gl_Position = positions[gl_VertexIndex % 3];
    
    gl_Position.y = -gl_Position.y;
}
