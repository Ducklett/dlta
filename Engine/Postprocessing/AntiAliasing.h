#pragma once

#include "./Effect.h"

namespace engine {
	namespace postprocessing {
		class AntiAliasing : public Effect {
		public:
			int quality;

			AntiAliasing() : Effect("antialias", "Antialiasing") { }

			void onEditorGui() {
				const char* QualityText[] = { "Low", "Medium", "High" };
				ImGui::Combo("Quality", &quality, QualityText, 3);
			}

			void onBind() {

			}
		};
	}
}
