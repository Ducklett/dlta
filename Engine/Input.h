#pragma once

#include <unordered_map>
#include "Deps.h"

namespace engine {

	using namespace glm;

	enum class Keycode {
		// keycodes taken directly from GLFW so they can be casted to those for now

		SPACE = 32,
		APOSTROPHE = 39  /* ' */,
		COMMA = 44  /* , */,
		MINUS = 45  /* - */,
		PERIOD = 46  /* . */,
		SLASH = 47  /* / */,
		ZERO = 48,
		ONE = 49,
		TWO = 50,
		THREE = 51,
		FOUR = 52,
		FIVE = 53,
		SIX = 54,
		SEVEN = 55,
		EIGHT = 56,
		NINE = 57,
		SEMICOLON = 59  /* ; */,
		EQUAL = 61  /* = */,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91  /* [ */,
		BACKSLASH = 92  /* \ */,
		RIGHT_BRACKET = 93  /* ] */,
		GRAVE_ACCENT = 96  /* ` */,
		WORLD_1 = 161 /* non-US #1 */,
		WORLD_2 = 162 /* non-US #2 */,

		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		NUMPAD0 = 320,
		NUMPAD1 = 321,
		NUMPAD2 = 322,
		NUMPAD3 = 323,
		NUMPAD4 = 324,
		NUMPAD5 = 325,
		NUMPAD6 = 326,
		NUMPAD7 = 327,
		NUMPAD8 = 328,
		NUMPAD9 = 329,
		NUMPAD_DECIMAL = 330,
		NUMPAD_DIVIDE = 331,
		NUMPAD_MULTIPLY = 332,
		NUMPAD_SUBTRACT = 333,
		NUMPAD_ADD = 334,
		NUMPAD_ENTER = 335,
		NUMPAD_EQUAL = 336,
		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		MENU = 348,
	};

	enum class KeyState {
		Depressed = 0,
		Pressed = 1,
	};

	struct Input {
		static GLFWwindow* window;

		static vec2 mouse;
		static vec2 mouseDelta;
		static std::unordered_map<int, int> keyState;

		static bool Key(Keycode c) {
			return glfwGetKey(window, (int)c) == GLFW_PRESS;
		}

		static bool KeyDown(Keycode c) {
			auto r = keyState.find((int)c);
			return  r != keyState.end() && r->second == (int)KeyState::Pressed;
		}

		static bool KeyUp(Keycode c) {
			auto r = keyState.find((int)c);
			return  r != keyState.end() && r->second == (int)KeyState::Depressed;
		}

		static void Update(GLFWwindow* window, vec2 size) {
			Input::window = window;

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			vec2 newmouse = vec2(xpos, ypos);

			mouseDelta = newmouse - mouse;
			mouse = newmouse;
		}

		static void key_change(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_REPEAT) return;
			keyState.insert({ key, action });
		}
	};

	GLFWwindow* Input::window = NULL;
	std::unordered_map<int, int> Input::keyState;
	vec2 Input::mouse = vec2(0, 0);
	vec2 Input::mouseDelta = vec2(0, 0);
}
