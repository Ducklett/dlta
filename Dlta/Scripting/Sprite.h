#pragma once

#include <Dlta/Core/Texture.h>
#include "RectTransform.h"

namespace dlta {
	struct Sprite {
		static Mesh spriteQuad;

		Texture tex;

		Sprite(const char* path) {
			tex = Texture::load(path, false, false, false);
			tex.bind(0);
		}

		Sprite(const char* path, RectTransform& tf) {
			tex = Texture::load(path, false, false, false);
			tex.bind(0);
			tf.size = tex.size;
		}
	};

	Mesh Sprite::spriteQuad;
}
