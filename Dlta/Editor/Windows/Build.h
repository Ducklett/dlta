#pragma once

#include "../EditorWindow.h"

namespace dlta {

	class BuildEditor : public EditorWindow {
	public:
		BuildEditor() : EditorWindow("Build settings") { includeInWindowList = false; }
		bool building;
		bool developmentBuild;
		int buildTarget;

		virtual void windowBegin() {
			auto& style = ImGui::GetStyle();
			auto pad = style.WindowPadding;
			style.WindowPadding = ImVec2(24, 24);

			if (ImGui::Begin(title, &open, ImGuiWindowFlags_AlwaysAutoResize)) {
				style.WindowPadding = pad;
				onGui();
			}
			windowEnd();
			style.WindowPadding = pad;
		}

		virtual void onGui() override {
			ImGui::Combo("Build target", &buildTarget, "Windows\0Linux\0web\0\0");
			ImGui::Checkbox("Development build", &developmentBuild);

			ImGui::Separator();

			if (ImGui::Button("Build")) buildProgram();

			ImGui::SameLine();

			if (ImGui::Button("Build and run")) buildProgram(true);
		}

		void buildProgram(bool run = false) {
			system("rm .\Build\cppengine.pdb");
			system("msbuild cppengine.sln /p:outDir=C:\\Users\\ivy\\Work\\cppengine\\Build;Configuration=Release");
			if (run) system(".\\Build\\cppengine.exe");
		}
	};
}
