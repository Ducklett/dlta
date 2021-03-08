#pragma once

#include "../Shader.h"
#include "./Transform.h"

namespace engine {
	struct MeshRenderer {
		Shader& shader;
		Transform& transform;
		GLuint mesh;

		MeshRenderer(Shader& shader, Transform& transform, GLuint mesh) :shader(shader), transform(transform), mesh(mesh) {}
	};
}