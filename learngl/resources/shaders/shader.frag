#version 410 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D uvgrid;
uniform sampler2D white_bear;

void main() {
    FragColor = vec4(lightColor * objectColor, 1.0);
}
