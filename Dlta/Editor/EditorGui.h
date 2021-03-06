#pragma once

#include "../Deps.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../postprocessing/EffectStack.h"
#include "EditorWindow.h"
#include "./Windows/About.h"
#include "./Windows/Build.h"
#include "./Windows/Preferences.h"
#include "./Theme.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

namespace dlta {
	static ImGuiIO* io;
	static AboutEditor* about;
	static PreferencesEditor* preferences;
	static BuildEditor* build;

	class EditorGUI {
	public:

		static void Init(GLFWwindow* window) {
			// TODO: check if this is correct..
			const char* glsl_version = "#version 330";

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			io = &ImGui::GetIO(); (void)io;
			//io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			//ImGui::StyleColorsDark();
			EditorTheme::themes[0].apply();
			//ApplyMidnightTheme();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);

			// Load Fonts
			// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
			// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
			// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
			// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
			// - Read 'docs/FONTS.md' for more instructions and details.
			// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
			//io.Fonts->AddFontDefault();
			io->Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu-Regular.ttf", 13.0f);
			//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
			//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
			//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
			//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
			//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
			//IM_ASSERT(font != NULL);

			// register permanent editor windows
			about = new AboutEditor();
			preferences = new PreferencesEditor();
			build = new BuildEditor();
		}

		static void Update(GLFWwindow* window, unsigned int gameTexId) {
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::DockSpaceOverViewport(NULL);

			static bool show_demo_window = true;

			if (ImGui::BeginMainMenuBar())
			{
				// placeholder menu items
				if (ImGui::MenuItem("File", "")) {
				}

				// TODO: more sophisticated system that lets you add windows into any of these sub menus
				if (ImGui::BeginMenu("Edit", "")) {
					if (ImGui::MenuItem(preferences->title)) preferences->open = !preferences->open;
					if (ImGui::MenuItem(build->title)) build->open = !build->open;
					ImGui::EndMenu();
				}
				// ===

				if (ImGui::BeginMenu("Window"))
				{
					if (ImGui::MenuItem("Demo window", "", show_demo_window)) show_demo_window = !show_demo_window;

					for (auto w : EditorWindow::editorWindows) {
						if (w->includeInWindowList) {
							if (ImGui::MenuItem(w->title, "", w->open)) w->open = !w->open;
						}
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("About", "")) about->open = true;

				ImGui::EndMainMenuBar();
			}

			if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

			for (auto w : EditorWindow::editorWindows) {
				if (w->open) w->windowBegin();
			}

			// Rendering
			ImGui::Render();
			/*int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);*/

			//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
			//glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			//glfwSwapBuffers(window);
		}

		static void Cleanup() {
			// Cleanup
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	};
}
