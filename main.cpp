
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

Mesh makeCube() {
	Mesh cube;

	cube.layout = { Vattr::XYZ, Vattr::UV, Vattr::NORMAL };

	cube.vertices = {
		// pos,               uv,         normal
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0,0,-1,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0,0,-1,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0,0,-1,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0,0,-1,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0,0,-1,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0,0,-1,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0,0,1,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0,0,1,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0,0,1,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0,0,1,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0,0,1,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0,0,1,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1,0,0,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1,0,0,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1,0,0,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1,0,0,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1,0,0,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1,0,0,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,0,0,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1,0,0,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1,0,0,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1,0,0,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1,0,0,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,0,0,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,-1,0,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0,-1,0,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0,-1,0,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0,-1,0,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0,-1,0,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,-1,0,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0,1,0,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0,1,0,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0,1,0,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0,1,0,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0,1,0,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0,1,0
	};

	cube.create();

	return move(cube);
}

Mesh makePlane() {
	Mesh plane;

	plane.layout = { Vattr::XYZ, Vattr::UV, Vattr::NORMAL };
	plane.vertices = {
		// bottom left
		-.5f,0,.5f, 0,0, 0,1,0,
		// top left
		-.5f,0,-.5f, 0,1, 0,1,0,
		// bottom right
		.5f,0,.5f, 1,0, 0,1,0,

		// top left
		-.5f,0,-.5f, 0,1, 0,1,0,
		// top right
		.5f,0,-.5f, 1,1, 0,1,0,
		// bottom right
		.5f,0,.5f, 1,0, 0,1,0,
	};

	plane.create();

	return move(plane);
}

int main()
{
	Application game(WIDTH, HEIGHT, "Pog");

	Mesh cubeMesh = makeCube();
	Mesh planeMesh = makePlane();

	Player pl = Player(game);

	Transform planeTransform = Transform();
	planeTransform.position = vec3(0, -1, 0);
	planeTransform.scale = vec3(20);
	MeshRenderer plane(game.testShader, planeTransform, planeMesh);

	Transform cubeTransform = Transform();
	MeshRenderer cube(game.testShader2, cubeTransform, cubeMesh);
	Transform cubeTransform2 = Transform();
	cubeTransform2.position += vec3(-1.5f, 0, 0);
	cubeTransform2.euler += vec3(10, 20, 30);
	cubeTransform2.scale = vec3(1.2f);
	MeshRenderer cube2(game.testShader, cubeTransform2, cubeMesh);

	game.entities.push_back(&pl);

	game.renderers.push_back(&cube);
	game.renderers.push_back(&cube2);
	game.renderers.push_back(&plane);

	game.run();

	return 0;
}

