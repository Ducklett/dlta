#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float iTime;

void main()
{    
    FragColor = textureLod(skybox, TexCoords, 0);
}
