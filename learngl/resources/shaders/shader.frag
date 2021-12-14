#version 410 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D uvgrid;
uniform sampler2D white_bear;

void main() {
    FragColor = texture(uvgrid, TexCoord) * (texture(white_bear, TexCoord) * 0.5 + 0.5);
}
