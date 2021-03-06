#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertcolor;
out vec2 uv;
out vec3 normal;
out vec3 position;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	uv = aUv;
	position = vec3(model*vec4(aPos, 1.0));
	// todo: move this to the cpu
	normal = normalize(mat3(transpose(inverse(model))) * aNormal);
}

