#pragma once

#include "Effect.h"

namespace engine {
	using namespace std;

	namespace postprocessing {
		class EffectStack {

		public:
			EffectStack() { }

			void push(Effect* e) {
				cout << "Pushing " << e->title << endl;
				stack.push_back(e);
			}

			void onEditorGui() {
				static bool pp_window = true;
				ImGui::Begin("Postprocessing", &pp_window);
				for (auto& e : stack) {
					ImGui::BeginGroup();
					ImGui::Text(e->title);
					e->onEditorGui();
					ImGui::EndGroup();
				}
				ImGui::End();
			}

			vector<Effect*> stack;
		};
	}
}
