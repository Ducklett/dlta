#pragma once

#include "../../vendor/imgui/imgui.h"

static void statOverlay()
{
	static bool statOverlay = true;

	// Top left 0, top right 1, bottom left 2, bottom right 3
	static int corner = 1;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
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
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.75f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay", &statOverlay, window_flags))
	{
		//ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Text("Stats");

		ImGui::Separator();

		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");

		ImGui::Separator();

		ImGui::Text("Frame time:      %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


		ImGui::Separator();

		/*if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (&statOverlay && ImGui::MenuItem("Close")) statOverlay = false;
			ImGui::EndPopup();
		}*/
	}
	ImGui::End();
}
