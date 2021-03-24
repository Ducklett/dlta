#pragma once

#include <Dlta/Deps.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace dlta {
	using namespace std;

	struct Texture {
		GLuint id = 0;
		int16_t unit = -1;

		static unsigned char* loadImageData(const char* path, int* width, int* height, int* channels, bool flip = true) {
			stbi_set_flip_vertically_on_load(flip);
			unsigned char* data = stbi_load(path, width, height, channels, 0);
			return data;
		}

		static void freeImageData(unsigned char* data) {
			stbi_image_free(data);
		}

		static Texture load(const char* path, bool filter = true, bool wrap = true, bool mipmap = true) {
			int width, height, nrChannels;
			unsigned char* data = loadImageData(path, &width, &height, &nrChannels);

			if (!data) return Texture{};

			Texture t = create(data, width, height, filter, wrap, mipmap);

			freeImageData(data);

			return move(t);
		}

		static Texture create_empty(int width, int height, bool filter = true, bool wrap = true, bool mipmap = true) {
			return move(create(NULL, width, height, filter, wrap, mipmap));
		}

		static Texture create(unsigned char* data, int width, int height, bool filter = true, bool wrap = true, bool mipmap = true) {

			// fixes color flickering in render texture in low ranges
			// TODO: parametarize format
			int format = !data ? GL_RGB16 : GL_RGB;

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			int wrapMode = wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;
			int filterMode = filter ? GL_LINEAR : GL_NEAREST;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

			if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);

			return { texture };
		}

		void bind(const int index) {
			unit = index;
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, id);
		}
	};
}
