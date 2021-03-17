#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float radius;
uniform float density;
uniform float falloff;

void main()
{ 
    vec2 center = TexCoords-.5;

    vec3 col = texture(screenTexture, TexCoords).rgb;
    float vignette = 1-max(0,pow(length(center)/radius, falloff) * density);
    col *= vignette;

    FragColor = vec4(col,1);
}
