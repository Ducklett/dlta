#pragma once

#include <Dlta/Core/Texture.h>
#include "RectTransform.h"

namespace dlta {
	struct Sprite {
		static Mesh spriteQuad;

		vec2 tiling = vec2(1, 1);
		vec2 offset = vec2(0, 0);
		vec2 size = vec2(1, 1);

		static Sprite slice(const Sprite& spr, vec2 tiling, vec2 offset, vec2 size = vec2(1, 1)) {
			Sprite copy(spr);
			copy.tiling = tiling;
			copy.offset = offset;
			copy.size = size;
			return copy;
		}

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
