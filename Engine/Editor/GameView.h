#pragma once
#include "../../vendor/imgui/imgui.h"

static bool gameViewOpen = true;

void gameView() {
	if (!gameViewOpen) return;

	auto& style = ImGui::GetStyle();
	auto pad = style.WindowPadding;
	style.WindowPadding = ImVec2(0,0);

	ImGui::Begin("Game View", &gameViewOpen);
	// TODO: feed in the real texture id 
	ImGui::Image((void*)(intptr_t)4, ImVec2(800, 600), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	style.WindowPadding = pad;
}
