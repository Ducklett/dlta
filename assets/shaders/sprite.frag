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

out vec4 FragColor;

void main() {
	vec4 col = texture(tex, uv);

	if (col.a<.2) discard;

	//col = vec3(1,.5,.1);
	//col = vec3(uv,0);


	FragColor = vec4(col.rgb,1);
}

