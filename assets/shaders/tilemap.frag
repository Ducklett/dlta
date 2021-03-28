#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 position;

uniform float iTime;
uniform vec3 iLight;
uniform vec3 iLightColor;
uniform vec3 iCamera;
uniform sampler2D tex;
uniform sampler2D tileset;
uniform samplerCube skybox;

out vec4 FragColor;

vec2 tileGv(vec2 uv, int id) {
	int dim = 2;
	float x = id%dim;
	float y = (id-x)/dim ;
	return vec2(uv.x/dim + x/dim, (uv.y)/dim + y/dim);
}

void main() {
	vec4 col = texture(tex, uv);


	int tileId = int(floor(col.r*4));

	FragColor = vec4(vec3(float(tileId)/4), 1);

	vec2 gv = fract(uv * vec2(512));
	gv = tileGv(gv, tileId);

	vec4 tile = texture(tileset, gv);

	if (tile.a<.2) discard;

	//col = vec3(1,.5,.1);
	//col = vec3(uv,0);


	FragColor = vec4(vec3(tile),1);
}

