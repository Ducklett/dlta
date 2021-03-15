#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

vec3 antiAlias(vec2 uv, vec2 pixel, sampler2D tex) {
    vec3 col = vec3(0);
    for(int x = -1;x<=1;x++) {
        for(int y = -1;y<=1;y++) {
            if (x ==0 && y == 0) continue;
            col += (texture(tex, uv+pixel*vec2(x,y)).rgb)/8.;
        }
    }
    return col;
}

//  float radius = .8;
//  float density = .3;
//  float falloff = 4.;
float vignette(vec2 center, float radius, float density, float falloff) {
    return 1-max(0,pow(length(center), falloff) * density);
}

void main()
{ 
    // vec2 px = (1/vec2(800,600))*.5;
    vec2 px = (1/vec2(800,600))*.5;

    // for (int x = -2;x<=2;x++) {

    // }

    float threshold = 1.;
    float transition = .6;
    float intensity = .5;

    // vec3 col = texture(screenTexture, TexCoords).rgb;
    vec3 col = antiAlias(TexCoords, px, screenTexture);

    // larger offset for bloom
    // should actually sample more pixels but I'm scared of frying the gpu :(
    px *=2.345;
    for(int x=-2; x<=2;x++) {
        for(int y=-2; y<=2;y++) {
            if (x==0&&y==0)continue;
            vec2 off = vec2(x,y);
            float offIntensity = 1./length(off);
            vec3 nb = texture(screenTexture, TexCoords+px*vec2(x,y)).rgb;
            float brigthness = smoothstep(threshold-transition,threshold, length(nb));
            float overshoot = max(0,brigthness - (threshold-transition));
            col += nb * (overshoot * intensity * offIntensity / 8.); // TODO: make less based on length (diagnoal less)?
        }
    }

    // vec3 col = texture(screenTexture, TexCoords).rgb;
    // float brigthness = smoothstep(threshold-transition,threshold, length(col));
    // float overshoot = max(0,brigthness - (threshold-transition));
    // col = col * (overshoot * intensity);

    // col = clamp(col,0,1);
    col *= vignette(TexCoords-.5, .2,1.,2.);

    FragColor = vec4(col,1);
}
