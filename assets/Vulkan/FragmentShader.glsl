// inputs
#ifdef NORMAL
    layout( location = NORMAL ) in vec4 normal_in;
#endif

#ifdef TEXTCOORD
    layout (location = TEXTCOORD ) in vec2 uv_in;
#endif

layout(location = 0) out vec4 fragment_color;

layout(set=4, binding=DIFFUSE_TINT) uniform DIFFUSE_TINT_NAME
{
    vec4 diffuseTint;
};

// binding sets the TEXTURE_UNIT value!
#ifdef DIFFUSE_SLOT
layout(set=5, binding=DIFFUSE_SLOT) uniform sampler2D myTex;
#endif

void main () {
    #ifdef DIFFUSE_SLOT
        vec4 col = texture(myTex, uv_in);
    #else
        vec4 col = vec4(1.0, 1.0, 0.0, 1.0);
    #endif
    
    fragment_color = col * vec4(diffuseTint.rgb, 1.0);
}
