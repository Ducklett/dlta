#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

out vec3 vertcolor;
out vec2 uv;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertcolor = aColor;
	uv = aUv;
}

