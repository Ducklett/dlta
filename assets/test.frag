#version 330 core

out vec4 FragColor;

in vec3 vertcolor;
in vec2 uv;

uniform vec4 color;
uniform sampler2D tex;

void main() {
	FragColor = texture(tex, uv);
}

