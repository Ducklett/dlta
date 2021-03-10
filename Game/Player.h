#pragma once

#include <iostream>
#include "../Engine/Application.h"
#include "../Engine/Scripting/Entity.h"

using namespace engine;

class Player : public Entity {
public:

	Application& app;
	Camera cam;
	float MovementSpeed = 2.5f;
	float MouseSensitivity = .1f;
	float scrollSpeed = 20;

	Player(Application& app) : app(app), cam(Camera(transform)) {
		transform.position = vec3(0, 0, 3);
		transform.euler.y = -90; // TODO: make it so objest look into the +z axis by default instead of +x(?)
		transform.euler.x = 0;
	}

	void start() {
		Camera::main = &cam;
	}

	bool wire = false;
	void update() {

		if (Input::KeyDown(Keycode::SPACE)) {
			wire = !wire;
			glPolygonMode(GL_FRONT_AND_BACK, wire ? GL_LINE : GL_FILL);
		}

		if (Input::Key(Keycode::ESCAPE)) {
			app.quit();
		}

		if (Input::Mouse(MouseButton::Right)) {
			transform.euler += vec3(Input::mouseDelta.y, Input::mouseDelta.x, 0) * MouseSensitivity;
		}

		if (transform.euler.x > 89.0f)
			transform.euler.x = 89.0f;
		if (transform.euler.x < -89.0f)
			transform.euler.x = -89.0f;

		transform.position += transform.forward() * Input::scrollDelta.y * scrollSpeed * Time::deltaTime;


		if (Input::Mouse(MouseButton::Middle)) {
			transform.position += transform.right() * -Input::mouseDelta.x * Time::deltaTime;
			transform.position += transform.up() * -Input::mouseDelta.y * Time::deltaTime;
		}
		else {
			float velocity = MovementSpeed * Time::deltaTime;
			if (Input::Key(Keycode::Q)) transform.position += vec3(0, -1, 0) * velocity;
			if (Input::Key(Keycode::E)) transform.position += vec3(0, 1, 0) * velocity;
			if (Input::Key(Keycode::W)) transform.position += transform.forward() * velocity;
			if (Input::Key(Keycode::A)) transform.position -= transform.right() * velocity;
			if (Input::Key(Keycode::S)) transform.position -= transform.forward() * velocity;
			if (Input::Key(Keycode::D)) transform.position += transform.right() * velocity;
		}

		//if (Input::Key(Keycode::SPACE)) {
			//cam.projection = Projection::Orthographic;
		glEnable(GL_FRAMEBUFFER_SRGB);
		/*}
		else {
			//cam.projection = Projection::Perspective;
			glDisable(GL_FRAMEBUFFER_SRGB);
		}*/

		// draw light
		Input::lightColor = Color::rgb((float)(sin(Time::time + 0) * .5 + .5), sin(Time::time + 1) * .5 + .5, sin(Time::time + 2) * .5 + .5);
		Gizmos::SetColor(Input::lightColor);
		Gizmos::wireSphere(Input::light, .2f);
		Input::light = vec3(sin(Time::time), 1, cos(Time::time));
	}
};
