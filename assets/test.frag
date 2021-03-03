#version 330 core

out vec4 FragColor;

in vec3 vertcolor;
in vec2 uv;

uniform vec4 color;
uniform sampler2D tex;
uniform sampler2D tex2;

void main() {
	FragColor =  mix(texture(tex, uv), texture(tex2, uv),.5) * vec4(vertcolor,1);
}

