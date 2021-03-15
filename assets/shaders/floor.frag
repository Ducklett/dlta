#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 position;

uniform float iTime;
uniform vec3 iLight;
uniform vec3 iLightColor;
uniform vec3 iCamera;
uniform sampler2D tex;
uniform sampler2D tex2;

out vec4 FragColor;

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

	vec3 result = (ambient + diffuse + specular) * albedo;
	return result;
}

float line(float d, float smoothing) {
  float lw = .01;
  return smoothstep(lw+smoothing,lw,abs(d-.5));
}

void main() {
	float viewDistance = 20;
	float visibility = length(vec3(iCamera-position)*vec3(1,1,.2))/viewDistance;

	// TODO: proper anti aliasing based on camera location/screen size
	// No idea how to do it properly as of right now...
	vec2 texelSize = vec2(1)/(800,600);
	vec2 uv = fract(position.xz);
	float mask = 0;
    for(int i = 0;i<3;i++) {
		float scale = pow(10.,float(i));
		vec2 smoothing = texelSize * scale;
        vec2 suv = fract(uv*scale+(scale==1?0:.5));

		float l = max(line(suv.x, smoothing.x),line(suv.y, smoothing.y));
		l -= visibility*pow(10, float(i+0));
        mask = max(mask,l);
    }

	mask -= visibility;

	vec3 col = vec3(0);

	FragColor = vec4(col,mask*.5);
}

