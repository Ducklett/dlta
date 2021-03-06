#pragma once

#include "Transform.h"

namespace engine {

	class Entity {
	public:
		bool active;
		Transform transform;

		virtual void start() = 0;
		virtual void update() = 0;
	};
}
