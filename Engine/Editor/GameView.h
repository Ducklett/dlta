#pragma once
#include "../../vendor/imgui/imgui.h"

static bool gameViewOpen = true;

void gameView(unsigned int textureId) {
	if (!gameViewOpen) return;

	auto& style = ImGui::GetStyle();
	auto pad = style.WindowPadding;
	style.WindowPadding = ImVec2(0, 0);

	ImGui::Begin("Game View", &gameViewOpen);
	// TODO: feed in the real texture id 

			//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (float)offset_x);
	const int gamewidth = 800;
	const int gameheight = 600;

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - gamewidth) * .5);
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() - gameheight) * .5);
	
	ImGui::Image((void*)(intptr_t)textureId, ImVec2(gamewidth, gameheight), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

	style.WindowPadding = pad;
}
