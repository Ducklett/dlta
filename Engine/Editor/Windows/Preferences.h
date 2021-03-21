#pragma once

#include<array>
#include "../../../vendor/imgui/imgui.h"
#include "../EditorWindow.h"
#include "../Theme.h"

namespace dlta {

	class PreferencesEditor : public EditorWindow {
	public:
		int selected = 0;

		const std::array<const char*, 3> sections = { "Appearance", "Foo", "Bar" };

		PreferencesEditor() : EditorWindow("Editor Preferences") { includeInWindowList = false; }

		virtual void onGui() override {

			// Left
			{
				ImGui::BeginChild("left pane", ImVec2(150, 0), true);
				int i = 0;
				for (auto& s : sections) {

					if (ImGui::Selectable(s, selected == i)) selected = i;

					i++;
				}
				ImGui::EndChild();
			}
			ImGui::SameLine();

			// Right
			{
				ImGui::BeginGroup();
				ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
				ImGui::Text("%s", sections[selected]);
				ImGui::Separator();

				// Appearance
				if (selected == 0) {
					static int themeIdx = 0;
					const char* themeNames[] = { "Midnight", "Cherry", "cyber" };
					ImGui::Combo("Theme", &themeIdx, themeNames, 3);

					if (ImGui::Checkbox("Compact mode", &editor_compact_mode)) EditorTheme::theme.apply();

					EditorTheme::theme.onGui();
				}
				else {
					ImGui::Text("placeholder");
				}
				ImGui::EndChild();
				ImGui::EndGroup();
			}
		}
	};
}
