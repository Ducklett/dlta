#pragma once

#include "../Deps.h"
#include "Transform.h"

namespace dlta {

	enum class Projection {
		Perspective,
		Orthographic,
	};

	class Camera
	{
	public:
		static Camera* main;
		Projection projection;
		Transform& transform;
		glm::vec3 WorldUp;
		float fov = 45;

		// constructor with vectors
		Camera(Transform& tf, Projection projection = Projection::Perspective, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : transform(tf), projection(projection)
		{
			WorldUp = up;
		}

		// returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(transform.position, transform.position + transform.forward(), transform.up());
		}

		glm::mat4 GetProjectionMatrix()
		{
			float near = 0.1f;
			float far = 100.0f;

			// TODO: inject aspect
			float width = 800;
			float height = 600;
			float aspect = width / height;
			float zoom = transform.position.z;

			switch (projection) {
			case Projection::Orthographic: return glm::ortho(-width / 2 * zoom, width / 2 * zoom, -height / 2 * zoom, height / 2 * zoom, 0.1f, 100.0f);
			case Projection::Perspective: return glm::perspective(glm::radians(fov), (float)width / (float)height, near, far);
			}

			// ortho based on screen pixels
			//return glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, 0.1f, 100.0f);

		}
	};

	Camera* Camera::main = NULL;
}
