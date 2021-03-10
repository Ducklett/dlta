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

float line(float d) {
  float lw = .01;
  float ls = .003;
  return smoothstep(lw+ls,lw,abs(d-.5));
}

void main() {
	float visibility = length(iCamera-position)/20;

	vec2 uv = fract(position.xz);
	float col = 0;
    for(int i = 0;i<2;i++) {
        vec2 suv = fract(uv*pow(10.,float(i)));
		float l = max(line(suv.x),line(suv.y));
		l -= visibility*pow(10, float(i+0));
        col = max(col,l);
    }

	col -= visibility;

	if (col<.2) discard;
	col*=.2;

	FragColor = vec4(vec3(col),1);//vec4(lighting(col, normal, position),1);
}

