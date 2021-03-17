#pragma once

#include "../../vendor/imgui/imgui.h"

void postProcessingEditor() {
	static bool pp_window = true;

	ImGui::Begin("Postprocessing", &pp_window);

	ImGui::BeginGroup();
	static float radius = .2;
	static float density = 1.;
	static float falloff = 2.;
	ImGui::Text("Vignette");
	ImGui::SliderFloat("Radius", &radius, 0, 2);
	ImGui::SliderFloat("Density", &density, .1f, 10);
	ImGui::SliderFloat("Falloff", &falloff, 1., 32);
	ImGui::EndGroup();

	ImGui::BeginGroup();
	static float threshold = 1.;
	static float transition = .6;
	static float intensity = .5;
	ImGui::Text("Bloom");
	ImGui::SliderFloat("Threshold", &threshold, 0, 2.);
	ImGui::SliderFloat("Transition", &transition, 0, 1.);
	ImGui::SliderFloat("Intensity", &intensity, .1, 2.);
	ImGui::EndGroup();

	ImGui::BeginGroup();
	static int quality = 0;
	ImGui::Text("AntiAliasing");
	const char* QualityText[] = { "Low", "Medium", "High" };
	ImGui::Combo("Quality", &quality, QualityText, 3);
	ImGui::EndGroup();

	ImGui::End();
}
