#pragma once

#include "../../../vendor/imgui/imgui.h"
#include "../EditorWindow.h"
#include "../../Postprocessing/EffectStack.h"

namespace dlta {
	using namespace postprocessing;

	class EffectEditor : public EditorWindow {
	public:
		EffectStack& effects;
		EffectEditor(EffectStack& effects) : EditorWindow("Postprocessing"), effects(effects) { open = true; }

		virtual void onGui() override {
			if (effects.stack.empty()) {
				ImGui::Text("No postprocessing effects registered");
			}
			else {
				for (auto& e : effects.stack) {
					ImGui::BeginGroup();
					ImGui::Text(e->title);
					e->onEditorGui();
					ImGui::EndGroup();
				}
			}

		}
	};
}
