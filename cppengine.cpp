
#include "Renderer.h"

using namespace engine;

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
	Renderer game(WIDTH, HEIGHT, "Pog");

	game.run();

	return 0;
}

