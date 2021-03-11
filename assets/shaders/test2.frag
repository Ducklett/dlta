#version 330 core

in vec2 uv;
in vec3 ro;
in vec3 hitPos;

uniform float iTime;
uniform vec3 iCamera;
uniform vec3 iLight;
uniform vec3 iLightColor;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform samplerCube skybox;

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

vec3 lighting(vec3 albedo, vec3 n, vec3 p) {
	vec3 lightDir = normalize(iLight - p);

	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * iLightColor;

	// diffuse
	float diff = max(dot(n, lightDir), 0);
	vec3 diffuse = diff*iLightColor;

	// specular
	float specularStrength = 1.0;
	vec3 viewDir = normalize(iCamera - p);
	vec3 reflectDir = reflect(-lightDir, n);
	float spec = pow(max(dot(viewDir, reflectDir),0), 32);
	vec3 specular = specularStrength * spec * iLightColor;

	// reflection
	float reflectivity = .8;
	float glossiness = .32;
	vec3 i = normalize(p - iCamera);
	vec3 r = reflect(i, n);
	const float lods = 8.;
	vec3 reflection = reflectivity * textureLod(skybox, r, lods-(lods*glossiness)).rgb;
	diffuse*=1-reflectivity;

	 vec3 result = (ambient + diffuse + specular + reflection) * albedo;

	return result;
}

void main() {
	vec3 rd = normalize(hitPos-ro);

	vec3 col;

	vec2 m = march(ro, rd);
	float d = m.x;
	float s = m.y;

	vec3 p = ro+rd*d;
	vec3 n = getNormal(ro+rd*d);
	 vec3 albedo = vec3(1,.5,.1);
	//vec3 albedo = vec3(1,1,1);
	//vec3 albedo = texture(tex, n.xy).xyz;
	col = lighting(albedo, n, p);

	FragColor = vec4(col, 1);
}

