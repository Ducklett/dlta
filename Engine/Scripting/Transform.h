#pragma once

#include "../Deps.h"

namespace engine {
	using namespace glm;

	struct Transform {
		vec3 position;
		vec3 euler;

		vec3 forward() {
			vec3 front = vec3(0);
			front.x = cos(radians(euler.y)) * cos(radians(euler.x));
			front.y = sin(radians(euler.x));
			front.z = sin(radians(euler.y)) * cos(radians(euler.x));
			front = normalize(front);

			if (front == vec3(0)) return vec3(0.0f, 0.0f, -1.0f);

			return front;
		}
		vec3 backward() { return -forward(); }
		// TODO: support z rotation (roll)
		vec3 right() { return normalize(cross(forward(), vec3(0, 1, 0))); }
		vec3 left() { return -right(); }
		vec3 up() { return normalize(cross(right(), forward())); }
		vec3 down() { return -up(); }
	};
}
