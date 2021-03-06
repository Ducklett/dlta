
#include "Engine/Application.h"
#include "Game/Player.h"

using namespace engine;

const int WIDTH = 800;
const int HEIGHT = 600;


int main()
{
	Application game(WIDTH, HEIGHT, "Pog");

	Player pl = Player(game);

	game.entities.push_back(&pl);

	game.run();

	return 0;
}

