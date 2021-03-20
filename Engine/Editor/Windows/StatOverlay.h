#pragma once

#include "../../../vendor/imgui/imgui.h"
#include "../EditorWindow.h"

namespace engine {
	class StatOverlay : public EditorWindow {
	public:
		StatOverlay() : EditorWindow("Stat overlay") { open = true; }

		virtual void windowBegin() override {
			// Top left 0, top right 1, bottom left 2, bottom right 3
			static int corner = 1;
			if (corner != -1)
			{
				const float PAD = 10.0f;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
				ImVec2 work_size = viewport->WorkSize;
				ImVec2 window_pos, window_pos_pivot;
				window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				ImGui::SetNextWindowViewport(viewport->ID);
			}

			ImGui::SetNextWindowBgAlpha(0.75f); // Transparent background

			ImGuiWindowFlags window_flags =
				ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

			if (ImGui::Begin(title, &open, window_flags)) onGui();
			windowEnd();
		}

		virtual void onGui() override {
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("Stats");

			ImGui::Separator();

			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse Position: <invalid>");

			ImGui::Separator();

			ImGui::Text("Frame time:      %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::Separator();
		}

		void windowEnd() { ImGui::End(); }
	};
}
