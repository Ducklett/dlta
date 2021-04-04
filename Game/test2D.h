#pragma once

#define DLTA_INTEGRATED_GRAPHICS

#include <Dlta/dlta.h>
#include "SceneCam2D.h"

using namespace dlta;


#if DLTA_EDITOR
const int WIDTH = 1440;
const int HEIGHT = 810;
#else
const int WIDTH = 800;
const int HEIGHT = 600;
#endif

Mesh makePlane() {
	Mesh plane;

	plane.layout = { Vattr::XY, Vattr::UV };
	plane.indexed = true;
	plane.vertices = {
		// bottom left
		-.5f,-.5f, 0,0,
		// top left
		-.5f,.5f, 0,1,
		// bottom right
		.5f,-.5f, 1,0,
		// top right
		.5f,.5f, 1,1,
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
	Application game(WIDTH, HEIGHT, "Pog", false);

	//game.skybox = Skybox("assets/cubemaps/sea", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag", ".jpg");
	//game.skybox = Skybox("assets/cubemaps/station", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag", ".png");

	// TODO: move this to engine internals?
	Shader spriteShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
	Shader mapShader("assets/shaders/tilemap.vert", "assets/shaders/tilemap.frag");

	// TODO: move this to engine internals
	Sprite::spriteQuad = makePlane();

	Sprite icon("assets/icon.png");
	TileMap map("assets/map.png", "assets/tileset.png");

	enum class Layer {
		Background,
		Foreground,
	};

	RectTransform planeTransform = RectTransform();
	planeTransform.position = vec3(0, 0, 0);
	planeTransform.size = vec2(600, 600);
	planeTransform.layer = (int)Layer::Foreground;
	SpriteRenderer plane(mapShader, planeTransform, map);

	Player pl = Player(game, planeTransform);

	game.entities.push_back(&pl);

	RectTransform tfs[8 * 6];
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 6; y++) {
			int id = x + y * 8;
			RectTransform& tf = tfs[id];
			tf.position = vec2(x * 100 - 400 + 50, y * 100 - 300 + 50);
			tf.size = vec2(50, 50);
			tf.layer = (int)Layer::Background;
			// just let it leak..
			auto sprr = new SpriteRenderer(spriteShader, tf, icon);
			game.renderers2d.push_back(sprr);
		}
	}

	game.renderers2d.push_back(&plane);

	game.run();

	return 0;
}

