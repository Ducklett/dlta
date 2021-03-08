
#include "Engine/Application.h"
#include "Game/Player.h"

using namespace engine;

const int WIDTH = 800;
const int HEIGHT = 600;

/*const glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};*/

int main()
{
	Application game(WIDTH, HEIGHT, "Pog");

	Player pl = Player(game);

	Transform cubeTransform = Transform();
	MeshRenderer cube(game.testShader2, cubeTransform, game.cube);
	Transform cubeTransform2 = Transform();
	cubeTransform2.position += vec3(-1.5f, 0, 0);
	cubeTransform2.euler += vec3(10, 20, 30);
	cubeTransform2.scale = vec3(1.2f);
	MeshRenderer cube2(game.testShader, cubeTransform2, game.cube);

	game.entities.push_back(&pl);

	game.renderers.push_back(&cube);
	game.renderers.push_back(&cube2);

	game.run();

	return 0;
}

