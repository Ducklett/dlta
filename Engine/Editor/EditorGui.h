#pragma once

#include "../Deps.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "PostProcessingEditor.h"
#include "StatOverlay.h"
#include "GameView.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// TODO: this used to be a reference, probably breaks shit now
static ImGuiIO *io;

class EditorGUI {
public:
	static void Init(GLFWwindow* window) {
		// TODO: check if this is correct..
		const char* glsl_version = "#version 330";

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO(); (void)io;
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ApplyMidnightTheme();
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
	}

	static void Update(GLFWwindow* window) {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		bool show_demo_window = true;
		if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

		postProcessingEditor();
		statOverlay();
		gameView();

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
private:
	static void ApplyMidnightTheme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.93f);
		colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 0.61f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.28f, 0.28f, 0.28f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.08f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 0.88f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.42f, 0.64f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.27f, 0.41f, 0.63f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.33f, 0.47f, 0.71f, 0.55f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.59f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.59f);

		// margin / padding / spacing
		style->WindowPadding = ImVec2(8, 8);
		style->FramePadding = ImVec2(8, 4);
		style->CellPadding = ImVec2(2, 2);
		style->ItemSpacing = ImVec2(6, 6);
		style->ItemInnerSpacing = ImVec2(6, 6);
		style->TouchExtraPadding = ImVec2(0, 0);
		style->IndentSpacing = 24;
		style->ScrollbarSize = 14;
		style->GrabMinSize = 8;

		// borders
		style->WindowBorderSize = 1;
		style->ChildBorderSize = 1;
		style->PopupBorderSize = 1;
		style->FrameBorderSize = 0;
		style->TabBorderSize = 0;

		// rounding
		style->WindowRounding = 2;
		style->ChildRounding = 2;
		style->FrameRounding = 4;
		style->PopupRounding = 4;
		style->ScrollbarRounding = 12;
		style->GrabRounding = 4;
		style->LogSliderDeadzone = 4;
		style->TabRounding = 4;
	}


};
