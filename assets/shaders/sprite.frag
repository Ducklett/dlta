#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 position;

uniform float iTime;
uniform vec3 iLight;
uniform vec3 iLightColor;
uniform vec3 iCamera;
uniform sampler2D tex;
// uniform sampler2D tex2;
uniform samplerCube skybox;

uniform vec2 tiling;
uniform vec2 offset;
uniform vec2 size;

out vec4 FragColor;

void main() {
	//vec2 tiling = vec2(8,8);
	//vec2 offset = vec2(4,mod(floor(iTime),4.));
	//vec2 size = vec2(4,1);

	vec2 off = vec2(offset.x, tiling.y-offset.y-1);
	off.y-=size.y-1;

	vec2 tv = uv/tiling*size+(off/tiling);
	vec4 col = texture(tex, tv);

	if (col.a<.2) discard;

	//col = vec3(1,.5,.1);
	//col = vec3(uv,0);


	FragColor = vec4(col.rgb,1);
}

