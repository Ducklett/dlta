#pragma once

#include "./Effect.h"

namespace dlta {
	namespace postprocessing {
		class Gamma : public Effect {
		public:
			glm::vec3 gamma = glm::vec3(.5, .5, .5);
			bool sync = true;

			Gamma() : Effect("gamma", "Gamma") { }

#if DLTA_EDITOR
			void onEditorGui() {
				if (ImGui::Checkbox("Sync channels ", &sync)) gamma = glm::vec3(gamma.x);

				if (sync) {
					float g = gamma.x;
					if (ImGui::SliderFloat("gamma", &g, .1f, 2.)) {
						gamma = glm::vec3(g);
					}
				}
				else {
					ImGui::SliderFloat("Red", &gamma.x, .1f, 2.);
					ImGui::SliderFloat("Green", &gamma.y, .1f, 2.);
					ImGui::SliderFloat("Blue", &gamma.z, .1f, 2.);
				}
			}
#endif

			void onBind() {
				shader.setVec3("gamma", gamma);
			}
		};
	}
}
