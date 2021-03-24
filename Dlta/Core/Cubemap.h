#pragma once

#include <Dlta/Deps.h>

#include "Texture.h"

namespace dlta {
	using namespace std;

	class Cubemap {
	public:
		unsigned int id = 0;

		Cubemap() {}
		Cubemap(const string& path, const string& extension = ".png") {

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 10);

			const vector<string> faces = { "right", "left", "top", "bottom", "front", "back" };

			for (int i = 0; i < faces.size(); i++) {
				const string texturePath = path + "/" + faces[i] + extension;

				int width, height, channels;
				unsigned char* data = Texture::loadImageData(texturePath.c_str(), &width, &height, &channels, false);

				if (data)
				{
					// target lod  internalformat width height  border providedFormat pixelType data
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				}
				else
				{
					std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
				}

				Texture::freeImageData(data);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	};
}
