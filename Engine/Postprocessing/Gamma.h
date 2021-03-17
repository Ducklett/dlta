#pragma once

#include "./Effect.h"

namespace engine {
	namespace postprocessing {
		class Gamma : public Effect {
		public:
			Gamma() : Effect("gamma") { }
		};
	}
}
