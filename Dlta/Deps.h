#pragma once

#ifndef DLTA_INTEGRATED_GRAPHICS
// prefer dedicated nvidia gpu
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// prefer dedicated amd gpu
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#if DLTA_EDITOR
#include <vendor/imgui/imgui.h>
#endif

