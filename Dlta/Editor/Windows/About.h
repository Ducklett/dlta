#pragma once

#include "../../../vendor/imgui/imgui.h"
#include "../EditorWindow.h"

namespace dlta {

	class AboutEditor : public EditorWindow {
	public:
		AboutEditor() : EditorWindow("About Dlta") { includeInWindowList = false; }

		virtual void windowBegin() {
			auto& style = ImGui::GetStyle();
			auto pad = style.WindowPadding;
			style.WindowPadding = ImVec2(64,64);

			if (ImGui::Begin(title, &open, ImGuiWindowFlags_AlwaysAutoResize)) {
				onGui();
			}
			windowEnd();
			style.WindowPadding = pad;
		}

		virtual void onGui() override {
			ImGui::Text("Dlta Engine");
			ImGui::Text("Version 0.0.0.1 - pre alpha");
		}
	};
}
