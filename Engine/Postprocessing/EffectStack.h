#pragma once

#include "Effect.h"

namespace engine {
	using namespace std;

	namespace postprocessing {
		class EffectStack {

		public:
			EffectStack() { }

			void push(Effect* e) {
				stack.push_back(e);
			}

			vector<Effect*> stack;
		};
	}
}
