
#include "Engine/Renderer.h"

using namespace engine;

Renderer* g;

const int WIDTH = 800;
const int HEIGHT = 600;



void Update() {
	Camera& cam = g->cam;

	/*const float lookSpeed = .1f * Time::deltaTime;
	cam.euler += vec3(Input::mouseDelta.y, Input::mouseDelta.x, 0) * lookSpeed;

	if (cam.euler.x < -80) {
		cam.euler.x = -80;
	}
	else if (cam.euler.x > 80) {
		cam.euler.x = 80;
	}*/
	cam.ProcessMouseMovement(Input::mouseDelta.x, Input::mouseDelta.y, true);

	if (Input::Key(Keycode::W)) cam.ProcessKeyboard(CameraMovement::FORWARD, Time::deltaTime);
	if (Input::Key(Keycode::A)) cam.ProcessKeyboard(CameraMovement::LEFT, Time::deltaTime);
	if (Input::Key(Keycode::S)) cam.ProcessKeyboard(CameraMovement::BACKWARD, Time::deltaTime);
	if (Input::Key(Keycode::D)) cam.ProcessKeyboard(CameraMovement::RIGHT, Time::deltaTime);

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
		glfwSetWindowShouldClose(Input::window, true);
	}

	float t = Time::time;
	Gizmos::SetColor(Color::rgb(
		(float)(sin(t) * .5 + .5),
		sin(t + 1.48) * .5 + .5,
		sin(t + 3.456) * .5 + .5));

	Gizmos::line(0, 0, cos(Time::time), sin(Time::time));
}

int main()
{
	Renderer game(WIDTH, HEIGHT, "Pog");

	//game.cam.position = vec3(0, 0, 3);
	g = &game;
	game.update_loop = Update;
	game.cam.MouseSensitivity = 30;
	game.run();

	return 0;
}

