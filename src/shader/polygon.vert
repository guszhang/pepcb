#version 330 core

uniform vec2 wOrigin;
uniform vec2 wSize;
uniform float scale;
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vCol;
out vec3 color;

void main() {
    gl_Position.xy = (wOrigin.xy + vPos * scale) / wSize * 2.0 - 1.0;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    color = vCol;
}