#shader VERTEX
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 color_v;

void main() {
    gl_Position = vec4(position, 1.0);
    color_v = color; 
}

#shader FRAGMENT
#version 410 core

in vec3 color_v;

out vec4 frag_color;

void main() {
    frag_color = vec4(color_v, 1.0);
}