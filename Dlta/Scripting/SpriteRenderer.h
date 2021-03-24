#pragma once

#include "../Shader.h"
#include "../Mesh.h"
#include "./RectTransform.h"

namespace dlta {
	struct SpriteRenderer {
		Shader& shader;
		RectTransform& transform;
		Mesh& mesh;
		bool ignoreDepth = false;

		SpriteRenderer(Shader& shader, RectTransform& transform, Mesh& mesh) :shader(shader), transform(transform), mesh(mesh) {}
	};
}
