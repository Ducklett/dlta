#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec2 uv = TexCoords;
    vec3 col = vec3(0);
    float samples = 8.;
    vec2 px = (1/vec2(800,600))*.5;

    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (x == 0 && y == 0) continue;
            col += (texture(screenTexture, uv + px * vec2(x, y)).rgb) / samples;
        }
    }

    FragColor = vec4(col, 1);
}
