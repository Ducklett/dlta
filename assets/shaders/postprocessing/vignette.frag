#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec2 center = TexCoords-.5;
    float radius = .2;
    float density = 4.;
    float falloff = 2.;

    vec3 col = texture(screenTexture, TexCoords).rgb;
    float vignette = 1-max(0,pow(length(center), falloff) * density);
    col *= vignette;

    FragColor = vec4(col,1);
}
