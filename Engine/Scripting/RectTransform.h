#pragma once

#include "../Deps.h"

namespace dlta {
	using namespace glm;

	struct RectTransform {
		vec2 position;
		float rotation;
		vec2 size = vec2(1);

		// TODO: bring back relative movement...
		/*vec3 right() { return normalize(cross(forward(), vec3(0, 1, 0))); }
		vec3 left() { return -right(); }
		vec3 up() { return normalize(cross(right(), forward())); }
		vec3 down() { return -up(); }*/
	};
}
