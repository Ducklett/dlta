#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

uniform vec3 iCamera;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertcolor;
out vec2 uv;
out vec3 hitPos;
out vec3 ro;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	uv = aUv;
	hitPos = aPos;
	ro = iCamera;
}

