#pragma once

#include "Deps.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine {
	using namespace std;

	struct Texture {
		GLuint id = 0;
		int unit = -1;

		static Texture load(const char* path, bool filter = true, bool wrap = true, bool mipmap = true) {
			stbi_set_flip_vertically_on_load(true);
			int width, height, nrChannels;
			unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);


			if (!data) return Texture{};

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			int wrapMode = wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;
			int filterMode = filter ? GL_LINEAR : GL_NEAREST;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

			if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);

			return { texture };
		}

		void bind(const int index) {
			unit = index;
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, id);
		}
	};
}
