#pragma once

#include "./Effect.h"

namespace engine {
	namespace postprocessing {
		class AntiAliasing : public Effect {
		public:
			AntiAliasing() : Effect("antialias") { }
		};
	}
}
