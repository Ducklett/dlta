#pragma once

#include "./Effect.h"

namespace engine {
	namespace postprocessing {
		class Vignette : public Effect {
		public:
			Vignette() : Effect("vignette") { }
		};
	}
}
