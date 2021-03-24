#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace dlta {
	namespace math {
		const double PI = 3.141592653589793;
		const double TAU = 6.283185307179586;
		const double RAD2DEG = 57.29577951308232;
		const double DEG2RAD = 0.017453292519943295;

		/*
		 * basic
		 */
		 // TODO: make these work for vectors..
		template <typename T> T min(T a, T b) { return a < b ? b : a; }
		template <typename T> T max(T a, T b) { return a > b ? b : a; }
		template <typename T> T clamp(T v, T minVal, T maxVal) { return max(minVal, min(maxVal, v)); }

		/*
		 * trig
		 */
		static inline double sin(double x) { return ::sin(x); }
		static inline float sin(float x) { return ::sin(x); }
		static inline double cos(double x) { return ::cos(x); }
		static inline float cos(float x) { return ::cos(x); }
		static inline double tan(double x) { return ::tan(x); }
		static inline float tan(float x) { return ::tan(x); }
		static inline double atan(double x) { return ::atan(x); }
		static inline float atan(float x) { return ::atan(x); }
		static inline double atan2(double y, double x) { return ::atan2(y, x); }
		static inline float atan2(float y, float x) { return ::atan2(y, x); }

		/*
		 * shadery
		 */
		template <typename T> T lerpUnclamped(T a, T b, float t) { return (1 - t) * a + t * b; }
		template <typename T> T static inline lerp(T a, T b, float t) { clamp(lerpUnclamped(a, b, t), 0, 1); }

		template <typename T> float inverseLerpUnclamped(T a, T b, T v) { return  (v - a) / (b - a); }
		template <typename T> static inline float inverseLerp(T a, T b, T v) { return  clamp(inverseLerpUnclamped(a, b, v), 0, 1); }

		// remap from one range to the other, unclamped
		template <typename T, typename U> U remapUnclamped(T inValue, T inMin, T inMax, U outMin, U outMax) {
			float t = inverseLerpUnclamped(inMin, inMax, inValue);
			return lerpUnclamped(outMin, outMax, t);
		}

		// remap from one range to the other, clamped
		template <typename T, typename U> U remap(T inValue, T inMin, T inMax, U outMin, U outMax) {
			float t = inverseLerp(inMin, inMax, inValue);
			return lerpUnclamped(outMin, outMax, t);
		}


		static inline int step(int v, int edge) { return v < edge ? 0 : 1; }
		static inline double step(double v, double edge) { return v < edge ? 0 : 1; }
		static inline float step(float v, float edge) { return v < edge ? 0 : 1; }
		static inline glm::vec2 step(glm::vec2 v, glm::vec2 edge) { return glm::vec2(step(v.x, edge.x), step(v.y, edge.y)); }
		static inline glm::vec3 step(glm::vec3 v, glm::vec3 edge) { return glm::vec3(step(v.x, edge.x), step(v.y, edge.y), step(v.y, edge.y)); }
	}
}
