#pragma once

#include "./Effect.h"

namespace dlta {
	namespace postprocessing {
		class AntiAliasing : public Effect {
		public:
			int quality;

			AntiAliasing() : Effect("antialias", "Antialiasing") { }

#ifdef DLTA_EDITOR
			void onEditorGui() {


				const char* QualityText[] = { "Low", "Medium", "High" };
				ImGui::Combo("Quality", &quality, QualityText, 3);
		}
#endif

			void onBind() {

			}
	};
}
}
