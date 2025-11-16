#version 410 core

layout (location = 0) in vec3 color;
layout (location = 1) in vec2 texcoord;

layout (location = 0) out vec4 frag_color;

uniform sampler3D test_texture_data;
uniform float sin_sq_time;

void main(void) {
	frag_color = texture(test_texture_data, vec3(texcoord, clamp(sin_sq_time, 0.1, 0.9)));
}