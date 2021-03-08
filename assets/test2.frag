#version 330 core

in vec2 uv;
in vec3 ro;
in vec3 hitPos;

uniform float iTime;
uniform vec3 iCamera;
uniform sampler2D tex;
uniform sampler2D tex2;

out vec4 FragColor;

#define MAX_STEPS 50
#define SURFACE_DIST 0.01
#define MAX_DIST 20

vec3 opRep( in vec3 p, in vec3 c)
{
    return mod(p+0.5*c,c)-0.5*c;
}

float sdBox( vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float getDist(vec3 p) {
	//p.xz += vec2(sin(iTime+p.y*10), cos(iTime+p.y*10))*.2;

	float domain = sdBox(p, vec3(.5));


	float amount = (mod(floor(iTime/2), 4))*2 + 1;

	vec3 q =  opRep(p, vec3(1/amount));

	float d = length(q)-(1/amount*.3);

	return max(domain,d*.7);
}

vec3 getNormal(vec3 p) {
	vec2 e = vec2(1e-2, 0);
	vec3 n = getDist(p) - vec3(
		getDist(p-e.xyy),
		getDist(p-e.yxy),
		getDist(p-e.yyx)
	);

	return normalize(n);
}

vec2 march(vec3 ro, vec3 rd) {
	float dO = 0.;
	float dS;
	int i;
	for (i = 0; i< MAX_STEPS; i++) {
		vec3 p = ro + dO*rd;	
		dS = getDist(p);
		dO += dS;
		if (dS<SURFACE_DIST || dO > MAX_DIST) break;
	}

	if (dS>SURFACE_DIST) discard;

	return vec2(dO, float(i)/MAX_STEPS);
}

void main() {
	vec3 rd = normalize(hitPos-ro);

	vec3 col;

	vec2 m = march(ro, rd);
	float d = m.x;
	float s = m.y;

	if (d < MAX_DIST) {
		vec3 n = getNormal(ro+rd*d);
		vec3 light = vec3(.2,.1,.5);
		vec3 shadow = vec3(.0,.0,.01);
		vec3 ambient = vec3(0.2f, 0.3f, 0.3f);
		float ambientStrength = .2f;
		vec3 lightPos = vec3(.5,1,0);
		float l = dot(n, lightPos);
		float ao = s*s;
		l -= ao;
		l = max(l,0);
		col = mix(shadow, texture(tex, n.xy).xyz, l); //mix(shadow, light, l);
		col += ambient * (1-l) * ambientStrength * (1-ao);
		col = max(col,0);
	}

	FragColor = vec4(col, 1);
}

