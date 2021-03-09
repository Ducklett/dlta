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

void main() {
	vec3 col = texture(tex, uv).rgb;
	vec3 logo = texture(tex2, uv).rgb;
	col = mix(col, col*logo, sin(iTime)*.5+.5);

	//col = vec3(1,.5,.1);

	FragColor = vec4(lighting(col, normal, position),1);
}

