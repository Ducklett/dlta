#pragma once

#include <Dlta/Dlta.h>
#include "SceneCam3D.h"

using namespace dlta;

const int WIDTH = 1440;
const int HEIGHT = 810;

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
	plane.indexed = true;
	plane.vertices = {
		// bottom left
		-.5f,0,.5f, 0,0, 0,1,0,
		// top left
		-.5f,0,-.5f, 0,1, 0,1,0,
		// bottom right
		.5f,0,.5f, 1,0, 0,1,0,
		// top right
		.5f,0,-.5f, 1,1, 0,1,0,
	};

	plane.indices = {
		0,1,2,
		1,3,2,
	};

	plane.create();

	return move(plane);
}

int game()
{
	Application game(WIDTH, HEIGHT, "Pog");

	game.skybox = Skybox("assets/cubemaps/sea", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag", ".jpg");
	//game.skybox = Skybox("assets/cubemaps/station", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag", ".png");

	// register postprocessing effects
	// TODO: unique_ptr craziness
	game.postProcessEffects.push(new postprocessing::Bloom());
	game.postProcessEffects.push(new postprocessing::AntiAliasing());
	game.postProcessEffects.push(new postprocessing::Vignette());
	game.postProcessEffects.push(new postprocessing::Gamma());


	Shader floorShader("assets/shaders/floor.vert", "assets/shaders/floor.frag");
	Mesh cubeMesh = makeCube();
	Mesh planeMesh = makePlane();

	Player pl = Player(game);

	Transform planeTransform = Transform();
	planeTransform.position = vec3(0, -1, 0);
	planeTransform.scale = vec3(20);
	MeshRenderer plane(floorShader, planeTransform, planeMesh);
	plane.ignoreDepth = true;

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

