#pragma once

#include <Dlta/Core/Shader.h>
#include <Dlta/Core/Mesh.h>

#include "Transform.h"

namespace dlta {
	struct MeshRenderer {
		Shader& shader;
		Transform& transform;
		Mesh& mesh;
		bool ignoreDepth = false;

		MeshRenderer(Shader& shader, Transform& transform, Mesh& mesh) :shader(shader), transform(transform), mesh(mesh) {}
	};
}