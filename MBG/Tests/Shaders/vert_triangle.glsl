#version 410 core

layout (location = 0) in vec3 vertex_triangle;
layout (location = 1) in vec3 color_vertex;

layout (location = 0) out vec3 color;

void main(void) {
				gl_Position = vec4(vertex_triangle, 1.0);
				color = color_vertex;
}