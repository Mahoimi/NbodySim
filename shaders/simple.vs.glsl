#version 330 core

layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal_coords;
layout(location = 2)in vec2 uv;

uniform mat4 MVP;

out vec3 vNormal;

void main(){
    gl_Position = MVP * vec4(position, 1.f);
    vNormal = normal_coords;
}
