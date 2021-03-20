#pragma once

#include "Transform.h"

namespace dlta {

	class Entity {
	public:
		bool active;
		Transform transform;

		virtual void start() = 0;
		virtual void update() = 0;
	};
}
