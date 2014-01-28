#version 330 core

in vec3 vNormal;

out vec4 Color;

void main(){
    Color = vec4(vNormal, 1.f);
}
