#pragma once

#include "../Shader.h"
#include "../Mesh.h"
#include "./Transform.h"

namespace engine {
	struct MeshRenderer {
		Shader& shader;
		Transform& transform;
		Mesh& mesh;

		MeshRenderer(Shader& shader, Transform& transform, Mesh& mesh) :shader(shader), transform(transform), mesh(mesh) {}
	};
}