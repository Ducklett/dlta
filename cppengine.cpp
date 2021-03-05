
#include "Engine/Renderer.h"

using namespace engine;

const int WIDTH = 800;
const int HEIGHT = 600;

float x = 0;
float y = 0;

void Update() {
	if (Input::Key(Keycode::W)) y += 0.01;
	if (Input::Key(Keycode::A)) x -= 0.01;
	if (Input::Key(Keycode::S)) y -= 0.01;
	if (Input::Key(Keycode::D)) x += 0.01;

	Gizmos::SetColor(Color::rgb(.2, .1, .7f));
	Gizmos::wireQuad(x, y, .1, .1);

	Gizmos::SetColor(Color::red);
	Gizmos::line(vec2(0), Input::mousePosition);
	Gizmos::SetColor(Color::cyan);
	Gizmos::wireCircle(Input::mousePosition, .5f);
}

int main()
{
	Renderer game(WIDTH, HEIGHT, "Pog");
	game.update_loop = Update;
	game.run();

	return 0;
}

