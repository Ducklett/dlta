#pragma once

#include <iostream>
#include <Dlta/Dlta.h>

using namespace dlta;

class Player : public Entity {
public:

	Application& app;
	Camera cam;
	float MovementSpeed = 250;
	float MouseSensitivity = .1f;
	float scrollSpeed = 20;
	RectTransform& test;
	Player(Application& app, RectTransform& test) : app(app), test(test), cam(Camera(transform)) {
		transform.position = vec3(0, 0, 1);
		transform.euler.y = -90; // TODO: make it so objest look into the +z axis by default instead of +x(?)
		transform.euler.x = 0;
		cam.projection = Projection::Orthographic;
	}

	void start() {
		Camera::main = &cam;
	}

	bool wire = false;
	void update() {

		// TOOD: maybe fix rotation so it goes the other way?
		// +deltaTime should rotate clockwise
		test.rotation -= Time::deltaTime;

		if (Input::KeyDown(Keycode::SPACE)) {
			wire = !wire;
		}
		glPolygonMode(GL_FRONT_AND_BACK, wire ? GL_LINE : GL_FILL);

		if (Input::Key(Keycode::ESCAPE)) {
			app.quit();
		}

		transform.position += transform.forward() * Input::scrollDelta.y * scrollSpeed * Time::deltaTime;
		transform.position.z = math::clamp(transform.position.z, .01f, 10.f);

		if (Input::Mouse(MouseButton::Middle) || (Input::Key(Keycode::LEFT_ALT))) {
			transform.position += transform.right() * -Input::mouseDelta.x * transform.position.z;
			transform.position += transform.up() * -Input::mouseDelta.y * transform.position.z;
		}
		else {
			float velocity = MovementSpeed * Time::deltaTime;
			if (Input::Key(Keycode::W)) transform.position += transform.up() * velocity;
			if (Input::Key(Keycode::S)) transform.position += transform.down() * velocity;
			if (Input::Key(Keycode::A)) transform.position += transform.left() * velocity;
			if (Input::Key(Keycode::D)) transform.position += transform.right() * velocity;
		}

		Gizmos::SetColor(Color::red);
		Gizmos::line(test.position, test.position + test.left() * vec2(100));
		Gizmos::SetColor(Color::green);
		Gizmos::line(test.position, test.position + test.up() * vec2(100));
		Gizmos::SetColor(Color::magenta);
		Gizmos::line(test.position, test.position + test.right() * vec2(100));
		Gizmos::SetColor(Color::yellow);
		Gizmos::line(test.position, test.position + test.down() * vec2(100));
	}
};
