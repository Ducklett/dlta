#pragma once

#include "./Effect.h"

namespace dlta {
	namespace postprocessing {
		class Bloom : public Effect {
		public:
			float threshold = 1.;
			float transition = .6;
			float intensity = .5;

			Bloom() : Effect("bloom", "Bloom") { }

#ifdef DLTA_EDITOR
			void onEditorGui() {
				ImGui::SliderFloat("Threshold", &threshold, 0, 2.);
				ImGui::SliderFloat("Transition", &transition, 0, 1.);
				ImGui::SliderFloat("Intensity", &intensity, .1, 2.);
			}
#endif

			void onBind() {
				shader.setFloat("threshold", threshold);
				shader.setFloat("transition", transition);
				shader.setFloat("intensity", intensity);
			}
		};
	}
}
