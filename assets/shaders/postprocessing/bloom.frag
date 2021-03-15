#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec2 px = (1/vec2(800,600))*2.381;

    float threshold = 1.;
    float transition = .6;
    float intensity = .5;

    vec3 col = texture(screenTexture, TexCoords).rgb;

    for(int x=-2; x<=2;x++) {
        for(int y=-2; y<=2;y++) {
            if (x==0&&y==0)continue;
            vec2 off = vec2(x,y);
            float offIntensity = 1./length(off);
            vec3 nb = texture(screenTexture, TexCoords+px*vec2(x,y)).rgb;
            float brigthness = smoothstep(threshold-transition,threshold, length(nb));
            float overshoot = max(0,brigthness - (threshold-transition));
            col += nb * (overshoot * intensity * offIntensity / 8.);
        }
    }

    FragColor = vec4(col,1);
}
