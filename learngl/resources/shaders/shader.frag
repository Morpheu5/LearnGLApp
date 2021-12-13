#version 410 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D uvgrid;
uniform sampler2D white_bear;

void main() {
    if (TexCoord.x < 0.5) {
        FragColor = texture(uvgrid, TexCoord);
    } else {
        FragColor = texture(white_bear, TexCoord);
    }
}
