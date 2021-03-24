#pragma once

#include "./Effect.h"

namespace dlta {
	namespace postprocessing {
		class Vignette : public Effect {
		public:
			float radius = 1.;
			float density = 1.;
			float falloff = 2.;

			Vignette() : Effect("vignette", "Vignette") { }

#ifdef DLTA_EDITOR
			void onEditorGui() {
				ImGui::SliderFloat("Radius", &radius, 0, 2);
				ImGui::SliderFloat("Density", &density, .1f, 10);
				ImGui::SliderFloat("Falloff", &falloff, 1., 32);
			}
#endif

			void onBind() {
				shader.setFloat("radius", radius);
				shader.setFloat("density", density);
				shader.setFloat("falloff", falloff);
			}
		};
	}
}
