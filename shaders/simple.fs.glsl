#version 330 core

uniform vec3 uColor;

out vec4 Color;

void main(){
    Color = vec4(uColor, 1.f);
}
