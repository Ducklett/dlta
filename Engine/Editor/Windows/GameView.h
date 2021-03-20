#pragma once
#include "../../../vendor/imgui/imgui.h"
#include "../EditorWindow.h"
#include "../../Input.h"

namespace dlta {

	class GameView : public EditorWindow {
	public:
		int screenTexId;

		GameView(int screenTexId) : EditorWindow("Game view"), screenTexId(screenTexId) { open = true; }

		virtual void onGui() override {
			auto& style = ImGui::GetStyle();
			auto pad = style.WindowPadding;
			style.WindowPadding = ImVec2(0, 0);

			// TODO: feed in real game dims that are set somewhere
			const int gamewidth = 800;
			const int gameheight = 600;

			float offsetX = (ImGui::GetWindowWidth() - gamewidth) * .5;
			float offsetY = ((ImGui::GetWindowHeight() - gameheight) * .5);

			auto pos = ImGui::GetWindowPos();

			Input::setMouseOffset(vec2(offsetX + pos.x, offsetY + pos.y));

			ImGui::SetCursorPosX(offsetX);
			ImGui::SetCursorPosY(offsetY);

			ImGui::Image((void*)(intptr_t)screenTexId, ImVec2(gamewidth, gameheight), ImVec2(0, 1), ImVec2(1, 0));

			style.WindowPadding = pad;
		}
	};
}
