#version 410 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    if (TexCoord.x < 0.5) {
        FragColor = texture(texture1, TexCoord);
    } else {
        FragColor = texture(texture2, TexCoord);
    }
}
