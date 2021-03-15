#pragma once

#include "./Effect.h"

namespace engine {
	namespace postprocessing {
		class Bloom : public Effect {
		public:
			Bloom() : Effect("bloom") { }
		};
	}
}
