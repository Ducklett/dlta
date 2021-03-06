#pragma once

namespace engine {
	struct Time {
		static float deltaTime;
		static float time;

		static void Update(float currentTime) {
			deltaTime = currentTime - time;
			time = currentTime;
		}
	};

	float Time::deltaTime = 0;
	float Time::time = 0;
}
