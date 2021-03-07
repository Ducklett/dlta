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

	void update() {
		if (Input::Key(Keycode::MINUS)) cam.fov -= Time::deltaTime * 100;
		if (Input::Key(Keycode::EQUAL)) cam.fov += Time::deltaTime * 100;
		if (Input::Key(Keycode::BACKSPACE)) cam.fov = 45;

		if (Input::Mouse(MouseButton::Right)) {
			transform.euler += vec3(-Input::mouseDelta.y, Input::mouseDelta.x, 0) * MouseSensitivity;
		}

		if (transform.euler.x > 89.0f)
			transform.euler.x = 89.0f;
		if (transform.euler.x < -89.0f)
			transform.euler.x = -89.0f;

		transform.position += transform.forward() * Input::scrollDelta.y * scrollSpeed * Time::deltaTime;


		if (Input::Mouse(MouseButton::Middle)) {
			transform.position += transform.right() * -Input::mouseDelta.x * Time::deltaTime;
			transform.position += transform.up() * Input::mouseDelta.y * Time::deltaTime;
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

		if (Input::Key(Keycode::SPACE)) {
			glEnable(GL_FRAMEBUFFER_SRGB);
		}
		else {
			glDisable(GL_FRAMEBUFFER_SRGB);
		}

		Gizmos::SetColor(Color::red);
		Gizmos::line(vec2(0), Input::mouse);
		Gizmos::SetColor(Color::cyan);
		Gizmos::line(vec2(0), Input::mouseDelta);
		Gizmos::wireCircle(vec2(0), glm::length(Input::mouseDelta));
		Gizmos::wireCircle(Input::mouse, .5f);

		if (Input::Key(Keycode::ESCAPE)) {
			app.quit();
		}

		float t = Time::time;
		Gizmos::SetColor(Color::rgb(
			(float)(sin(t) * .5 + .5),
			sin(t + 1.48) * .5 + .5,
			sin(t + 3.456) * .5 + .5));

		Gizmos::line(0, 0, cos(Time::time), sin(Time::time));
	}
};
