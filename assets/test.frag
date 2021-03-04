#version 330 core

in vec2 uv;

uniform float iTime;
uniform sampler2D tex;
uniform sampler2D tex2;

out vec4 FragColor;

void main() {
	vec3 col = texture(tex, uv).rgb;
	vec3 logo = texture(tex2, uv).rgb;
	col = mix(col, col*logo, sin(iTime)*.5+.5);
	FragColor = vec4(col,1);
}

