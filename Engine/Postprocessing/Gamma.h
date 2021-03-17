#pragma once

#include "./Effect.h"

namespace engine {
	namespace postprocessing {
		class Gamma : public Effect {
		public:
			Gamma() : Effect("gamma", "Gamma") { }

			void onEditorGui() {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "hmm.. curvy :))))");
			}

			void onBind() {}
		};
	}
}
