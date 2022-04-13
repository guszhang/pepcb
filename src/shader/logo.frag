#version 460 core

in vec2 tex_coords;
out vec4 fColor;
uniform sampler2D tex_image;

void main(){
    fColor=texture(tex_image, tex_coords);
}