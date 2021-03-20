#pragma once

#include "../../vendor/imgui/imgui.h"
#include "EditorWindow.h"
#include "../Postprocessing/EffectStack.h"

namespace engine {
	using namespace postprocessing;

	class EffectEditor : public EditorWindow {
	public:
		EffectStack& effects;
		EffectEditor(EffectStack& effects) : EditorWindow("Postprocessing"), effects(effects) { open = true; }

		virtual void onGui() override {
			for (auto& e : effects.stack) {
				ImGui::BeginGroup();
				ImGui::Text(e->title);
				e->onEditorGui();
				ImGui::EndGroup();
			}
		}
	};
}
