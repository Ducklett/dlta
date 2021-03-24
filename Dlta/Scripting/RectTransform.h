#pragma once

#include <Dlta/Deps.h>
#include <Dlta/Core/Math.h>

namespace dlta {
	using namespace glm;

	struct RectTransform {
		vec2 position;
		float rotation;
		vec2 size = vec2(1);

		vec2 right() { return normalize(vec2(cos(rotation), sin(rotation))); }
		vec2 left() { return -right(); }
		vec2 up() { return normalize(vec2(cos(rotation - math::PI / 2), sin(rotation - math::PI / 2))); }
		vec2 down() { return -up(); }
	};
}
