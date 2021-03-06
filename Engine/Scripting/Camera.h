#pragma once

#include "../Deps.h"
#include "Transform.h"

namespace engine {

	class Camera
	{
	public:
		static Camera* main;
		Transform& transform;
		glm::vec3 WorldUp;
		float fov = 45;

		// constructor with vectors
		Camera(Transform& tf, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : transform(tf)
		{
			WorldUp = up;
		}

		// returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(transform.position, transform.position + transform.forward(), transform.up());
		}
	};

	Camera* Camera::main = NULL;
}
