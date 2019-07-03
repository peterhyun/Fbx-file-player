#version 330 core
out vec4 FragColor;

uniform vec3 lightDirection;
uniform vec3 viewPos;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

in vec3 worldNormal;
in vec2 TexCoord;
in vec3 worldPos;

void main()
{
vec3 ambient = vec3(0.3) * vec3(texture(diffuseTexture, TexCoord));

vec3 norm = normalize(worldNormal);
vec3 lightDir = normalize(lightDirection);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = vec3(0.8) * diff * vec3(texture(diffuseTexture, TexCoord));

vec3 viewDir = normalize(viewPos-worldPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 10.0);
vec3 specular = vec3(0.5) * spec * vec3(texture(specularTexture, TexCoord));

FragColor = vec4(ambient + diffuse + specular, 1.0);
}
