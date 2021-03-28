#pragma once

#include <Dlta/Core/Texture.h>
#include "RectTransform.h"
#include "Sprite.h"

namespace dlta {
	struct TileMap : public Sprite {

		Texture tileset;

		TileMap(const char* path, const char* tilesetPath) : Sprite(path) {
			tileset = Texture::load(tilesetPath, false, false, false);
			tileset.bind(1);
		}
	};
}
