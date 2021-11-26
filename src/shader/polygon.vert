#version 110

uniform vec2 wOrigin;
uniform vec2 wSize;
uniform float scale;
attribute vec2 vPos;
varying vec3 color;

void main() {
    gl_Position.xy = vPos.xy / scale; //(wOrigin.xy + vPos * scale) / wSize * 2.0 - 1.0;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    color = vec3(1.0, 0.0, 0.0);
}