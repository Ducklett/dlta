#pragma once

#include <vector>
#include "../Deps.h"

namespace engine {
	using namespace std;

	class EditorWindow {
	public:
		static vector<EditorWindow*> editorWindows;

		bool open = false;
		const char* title;

		EditorWindow(const char* title) : title(title) {
			editorWindows.push_back(this);
		}

		virtual void windowBegin() {
			if (ImGui::Begin(title, &open, ImGuiWindowFlags_None)) {
				onGui();
			}
			windowEnd();
		}

		virtual void onGui() = 0;

		void windowEnd() { ImGui::End(); }

	};

	vector<EditorWindow*> EditorWindow::editorWindows;
}
