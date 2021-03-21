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

					EditorTheme& theme = EditorTheme::themes[editor_theme_index];

					// TODO: iterate themes
					const char* themeNames[] = { "Midnight", "Cherry", "cyber" };

					int lastIndex = editor_theme_index;
					if (ImGui::Combo("Theme", &editor_theme_index, themeNames, 3)) {
						printf("Theme changed");
						EditorTheme::themes[editor_theme_index].apply(false, lastIndex);
					}

					if (ImGui::Checkbox("Compact mode", &editor_compact_mode)) {
						theme.apply(true);
					}

					theme.onGui();
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
