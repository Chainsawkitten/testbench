// buffer inputs
/*#ifdef NORMAL
	layout(binding=NORMAL) buffer nor { vec4 normal_in[]; };
	layout(location=NORMAL) out vec4 normal_out;
#endif

#ifdef TEXTCOORD
	layout(binding=TEXTCOORD) buffer text { vec2 uv_in[]; };
	layout(location=TEXTCOORD) out vec2 uv_out;
#endif
layout(binding=POSITION) buffer pos { vec4 position_in[]; };*/

// uniform block
// layout(std140, binding = 20) uniform TransformBlock
// {
//  	vec4 tx;
// } transform;

layout(binding=TRANSLATION) buffer TRANSLATION_NAME {
	vec4 translate;
};

layout(binding=POSITION) buffer Position {
	vec4 position;
};

#ifdef NORMAL
layout(binding=NORMAL) buffer Normal {
	vec4 normal;
};
layout(location=NORMAL) out vec4 normal_out;
#endif TEXTCOORD

#ifdef TEXTCOORD
	layout(binding=TEXTCOORD) buffer Textcoord {
		vec2 UV;
	};
	layout(location=TEXTCOORD) out vec2 uv_out;
#endif

void main() {
	#ifdef NORMAL
		normal_out = normal_in[gl_VertexIndex];
	#endif

	#ifdef TEXTCOORD
		uv_out = uv_in[gl_VertexIndex];
	#endif

	//gl_Position = position_in[gl_VertexIndex] + translate;
	
    
    gl_Position = position + translate;
}
