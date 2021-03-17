#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec3 col = texture(screenTexture, TexCoords).rgb;
	col = clamp(col,vec3(0), vec3(1));
    col = sqrt(col);
    FragColor = vec4(col,1);
}
