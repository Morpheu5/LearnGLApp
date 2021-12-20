#version 410 core

out vec4 FragColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform sampler2D uvgrid;
uniform sampler2D white_bear;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPos);
    float d = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = d * lightColor;

    // Specular
    float specularStrength = 0.5;
    float shininess = 128.0;
    vec3 viewerDirection = normalize(cameraPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float s = pow(max(dot(viewerDirection, reflectDirection), 0.0), shininess);
    vec3 specular = specularStrength * s * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
