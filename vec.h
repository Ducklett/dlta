#pragma once

namespace engine {

	using namespace std;

	struct Color {
		float r;
		float g;
		float b;
		float a;


		/// <summary>
		/// create color from rgba values 0.0-1.0
		/// </summary>
		inline static Color rgba(float r, float g, float b, float a) {
			return Color{ r, g, b, a };
		}

		/// <summary>
		/// create color from rgb values 0.0-1.0
		/// </summary>
		inline static Color rgb(float r, float g, float b) {
			return rgba(r, g, b, 1);
		}

		/// <summary>
		/// create color from rgb values 0-255
		/// </summary>
		inline static Color rgb(int r, int g, int b) {
			return rgba((float)r / 255, (float)g / 255, (float)b / 255, 1);
		}

		static Color red;
		static Color green;
		static Color blue;
		static Color cyan;
		static Color yellow;
		static Color magenta;
		static Color clear;
	};


	Color Color::red = Color{ .87f,.10f,.19f,1 };
	Color Color::green = Color{ .10f,.87f,.29f,1 };
	Color Color::blue = Color{ .10f,.19f,.87f,1 };
	Color Color::cyan = Color{ .10f,.87f,.84f,1 };
	Color Color::yellow = Color{ .92f,.81f,.35f,1 };
	Color Color::magenta = Color{ .64f,.20f,.81f,1 };
	Color Color::clear = Color{ 0,0,0,0 };
}
