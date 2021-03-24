#pragma once

#include <Dlta/Core/Shader.h>
#include <Dlta/Core/Mesh.h>

#include "RectTransform.h"

namespace dlta {
	struct SpriteRenderer {
		Shader& shader;
		RectTransform& transform;
		Mesh& mesh;
		bool ignoreDepth = false;

		SpriteRenderer(Shader& shader, RectTransform& transform, Mesh& mesh) :shader(shader), transform(transform), mesh(mesh) {}
	};
}
