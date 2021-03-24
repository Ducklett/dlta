#pragma once

#include <Dlta/dlta.h>
#include "Player.h"

using namespace dlta;

const int WIDTH = 1440;
const int HEIGHT = 810;

Mesh makePlane() {
	Mesh plane;

	plane.layout = { Vattr::XY, Vattr::UV };
	plane.indexed = true;
	plane.vertices = {
		// bottom left
		-.5f,.5f, 0,0,
		// top left
		-.5f,-.5f, 0,1,
		// bottom right
		.5f,.5f, 1,0,
		// top right
		.5f,-.5f, 1,1,
	};

	plane.indices = {
		0,1,2,
		1,3,2,
	};

	plane.create();

	return move(plane);
}

int test2D()
{
	Application game(WIDTH, HEIGHT, "Pog");

	//game.skybox = Skybox("assets/cubemaps/sea", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag", ".jpg");
	//game.skybox = Skybox("assets/cubemaps/station", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag", ".png");

	Shader spriteShader("assets/shaders/test.vert", "assets/shaders/test.frag");
	Mesh planeMesh = makePlane();

	Player pl = Player(game, true);

	RectTransform planeTransform = RectTransform();
	planeTransform.position = vec3(0,0,0);
	planeTransform.size = vec2(100,100);
	SpriteRenderer plane(spriteShader, planeTransform, planeMesh);

	game.entities.push_back(&pl);

	game.renderers2d.push_back(&plane);

	game.run();

	return 0;
}

