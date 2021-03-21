#pragma once

#include "../../vendor/imgui/imgui.h"

namespace dlta {
	static bool editor_compact_mode = false;
	static int editor_theme_index = 0;

	struct EditorTheme {
		static EditorTheme themes[];
		static EditorTheme referenceTheme;

		const char* title;

		ImVec4 primary = ImVec4(0.33f, 0.47f, 0.71f, 1.00f);
		ImVec4 primaryHover = ImVec4(0.27f, 0.42f, 0.64f, 1.00f);
		ImVec4 bgDark = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		ImVec4 bg = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		ImVec4 bgLight = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		ImVec4 bgLighter = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		ImVec4 bgLightest = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		ImVec4 text = ImVec4(1.00f, 1.00f, 1.00f, 0.93f);
		ImVec4 textDisabled = ImVec4(1.00f, 1.00f, 1.00f, 0.61f);

		void apply(bool softreset = false, int lastThemeIndex = -1) {
			ImGuiStyle* style = &ImGui::GetStyle();
			ImVec4* colors = style->Colors;


			colors[ImGuiCol_Text] = text;
			colors[ImGuiCol_TextDisabled] = textDisabled;
			colors[ImGuiCol_WindowBg] = bg;
			colors[ImGuiCol_ChildBg] = ImVec4(0.28f, 0.28f, 0.28f, 0.00f);
			colors[ImGuiCol_PopupBg] = bgDark;
			colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.08f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = bgDark;
			colors[ImGuiCol_FrameBgHovered] = bgLight;
			colors[ImGuiCol_FrameBgActive] = bgLighter;
			colors[ImGuiCol_TitleBg] = bg;
			colors[ImGuiCol_TitleBgActive] = bg;
			colors[ImGuiCol_TitleBgCollapsed] = bg;
			colors[ImGuiCol_MenuBarBg] = bg;
			colors[ImGuiCol_ScrollbarBg] = bgDark;
			colors[ImGuiCol_ScrollbarGrab] = bgLighter;
			colors[ImGuiCol_ScrollbarGrabHovered] = bgLightest;
			colors[ImGuiCol_ScrollbarGrabActive] = bgLightest;
			colors[ImGuiCol_CheckMark] = text;
			colors[ImGuiCol_SliderGrab] = primary;
			colors[ImGuiCol_SliderGrabActive] = primary;
			colors[ImGuiCol_Button] = primary;
			colors[ImGuiCol_ButtonHovered] = ImVec4(primaryHover);
			colors[ImGuiCol_ButtonActive] = ImVec4(primaryHover);
			colors[ImGuiCol_Header] = bgLight;
			colors[ImGuiCol_HeaderHovered] = bgLight;
			colors[ImGuiCol_HeaderActive] = bgLightest;
			colors[ImGuiCol_Separator] = bgLighter;
			colors[ImGuiCol_SeparatorHovered] = bgLightest;
			colors[ImGuiCol_SeparatorActive] = primary;
			colors[ImGuiCol_ResizeGrip] = textDisabled;
			colors[ImGuiCol_ResizeGripHovered] = text;
			colors[ImGuiCol_ResizeGripActive] = primary;
			colors[ImGuiCol_Tab] = bgDark;
			colors[ImGuiCol_TabHovered] = bgLightest;
			colors[ImGuiCol_TabActive] = bgLight;
			colors[ImGuiCol_TabUnfocused] = bgDark;
			colors[ImGuiCol_TabUnfocusedActive] = bg;
			colors[ImGuiCol_PlotLines] = textDisabled;
			colors[ImGuiCol_PlotLinesHovered] = primary;
			colors[ImGuiCol_PlotHistogram] = text;
			colors[ImGuiCol_PlotHistogramHovered] = primary;
			colors[ImGuiCol_TableHeaderBg] = bg;
			colors[ImGuiCol_TableBorderStrong] = bgLightest;
			colors[ImGuiCol_TableBorderLight] = bgLighter;
			colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
			colors[ImGuiCol_TextSelectedBg] = primary;
			colors[ImGuiCol_DragDropTarget] = primary;
			colors[ImGuiCol_NavHighlight] = primary;
			colors[ImGuiCol_NavWindowingHighlight] = primary;
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.59f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.59f);

			// margin / padding / spacing
			if (editor_compact_mode) {
				style->WindowPadding = ImVec2(4, 4);
				style->FramePadding = ImVec2(8, 4);
				style->CellPadding = ImVec2(2, 2);
				style->ItemSpacing = ImVec2(4, 4);
				style->ItemInnerSpacing = ImVec2(4, 4);
				style->TouchExtraPadding = ImVec2(0, 0);
				style->IndentSpacing = 12;
				style->ScrollbarSize = 12;
				style->GrabMinSize = 8;
			}
			else {
				style->WindowPadding = ImVec2(8, 8);
				style->FramePadding = ImVec2(8, 4);
				style->CellPadding = ImVec2(2, 2);
				style->ItemSpacing = ImVec2(6, 6);
				style->ItemInnerSpacing = ImVec2(6, 6);
				style->TouchExtraPadding = ImVec2(0, 0);
				style->IndentSpacing = 24;
				style->ScrollbarSize = 14;
				style->GrabMinSize = 8;
			}

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

			if (!softreset) {
				if (lastThemeIndex > -1) themes[lastThemeIndex] = referenceTheme;
				referenceTheme = *this;
			}
		}

		void onGui() {
			ImGuiStyle style = ImGui::GetStyle();

			const char* styleNames[] = {
				"Primary", "Primary hover", "Background dark", "Background", "Background light",
				"Background lighter", "Background lightest", "Text", "Text disabled" };

			ImVec4* stylePointers[] = { &primary, &primaryHover, &bgDark, &bg, &bgLight, &bgLighter, &bgLightest, &text, &textDisabled };
			ImVec4* ref[] = { &referenceTheme.primary, &referenceTheme.primaryHover, &referenceTheme.bgDark, &referenceTheme.bg,
				&referenceTheme.bgLight, &referenceTheme.bgLighter, &referenceTheme.bgLightest, &referenceTheme.text, &referenceTheme.textDisabled };

			for (int i = 0; i < 9; i++) {
				auto p = stylePointers[i];

				const char* name = styleNames[i];
				ImGui::PushID(i);
				ImGui::ColorEdit4("##color", (float*)p, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
				if (memcmp(p, ref[i], sizeof(ImVec4)) != 0)
				{
					// Tips: in a real user application, you may want to merge and use an icon font into the main font,
					// so instead of "Save"/"Revert" you'd use icons!
					// Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { *(ref[i]) = *(stylePointers[i]); }
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { *(stylePointers[i]) = *(ref[i]); }
				}
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				ImGui::TextUnformatted(name);
				ImGui::PopID();
			}
			apply(true);
		}
	};

	EditorTheme EditorTheme::referenceTheme;
	EditorTheme EditorTheme::themes[] = {
		{
			 "Midnight",
			ImVec4(0.33f, 0.47f, 0.71f, 1.00f),
			ImVec4(0.27f, 0.42f, 0.64f, 1.00f),
			ImVec4(0.16f, 0.16f, 0.16f, 1.00f),
			ImVec4(0.18f, 0.18f, 0.18f, 1.00f),
			ImVec4(0.24f, 0.24f, 0.24f, 1.00f),
			ImVec4(0.28f, 0.28f, 0.28f, 1.00f),
			ImVec4(0.35f, 0.35f, 0.35f, 1.00f),
			ImVec4(1.00f, 1.00f, 1.00f, 0.93f),
			ImVec4(1.00f, 1.00f, 1.00f, 0.61f),
		},
		{
			"Cherry",
			ImVec4(0.8980392156862745f,0.48627450980392156f,0.6627450980392157f,1),
			ImVec4(0.8117647058823529f,0.30196078431372547f,0.6274509803921569f,1),
			ImVec4(0.8470588235294118f,0.8470588235294118f,0.8470588235294118f,1),
			ImVec4(0.8980392156862745f,0.8980392156862745f,0.8980392156862745f,1),
			ImVec4(0.9450980392156862f,0.9450980392156862f,0.9450980392156862f,1),
			ImVec4(0.9607843137254902f,0.9607843137254902f,0.9607843137254902f,1),
			ImVec4(1.f,1.f,1.f,1),
			ImVec4(0.2196078431372549f,0.17254901960784313f,0.20392156862745098f,1),
			ImVec4(0.1607843137254902f,0.058823529411764705f,0.12549019607843137f,1),
		},
		{
			"Cyber",
			ImVec4(0.33f, 0.87f, 0.31f, 1.00f),
			ImVec4(0.27f, 0.82f, 0.34f, 1.00f),
			ImVec4(0.16f, 0.16f, 0.16f, 1.00f),
			ImVec4(0.18f, 0.18f, 0.18f, 1.00f),
			ImVec4(0.24f, 0.24f, 0.24f, 1.00f),
			ImVec4(0.28f, 0.28f, 0.28f, 1.00f),
			ImVec4(0.35f, 0.35f, 0.35f, 1.00f),
			ImVec4(1.00f, 1.00f, 1.00f, 0.93f),
			ImVec4(1.00f, 1.00f, 1.00f, 0.61f),
		},
	};
}
