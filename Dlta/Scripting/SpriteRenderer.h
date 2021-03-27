#pragma once

#include <Dlta/Core/Shader.h>
#include <Dlta/Core/Mesh.h>

#include "Sprite.h"
#include "RectTransform.h"

namespace dlta {
	struct SpriteRenderer {
		Shader& shader;
		RectTransform& transform;
		Sprite& sprite;
		bool ignoreDepth = false;

		SpriteRenderer(Shader& shader, RectTransform& transform, Sprite& sprite) :shader(shader), transform(transform), sprite(sprite) {}
	};
}
