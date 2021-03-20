#pragma once

#include "Deps.h"
#include "Cubemap.h"
#include "Mesh.h"
#include "./Scripting/Camera.h"
#include <vector>

namespace dlta {
	using namespace std;

	class Skybox {
	public:
		Cubemap cubemap;
		Mesh cubeMesh;
		Shader shader;

		Skybox() {}

		Skybox(const string& cubemapPath, const string& vertexPath, const string& fragmentPath, const string& extension = ".png") {
			shader = Shader(vertexPath.c_str(), fragmentPath.c_str());
			shader.disableTranslation = true;
			cubemap = Cubemap(cubemapPath, extension);

			cubeMesh.layout = { Vattr::XYZ };
			cubeMesh.vertices = {
				// positions          
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};
			cubeMesh.create();
		}

		void draw() {
			glDepthFunc(GL_LEQUAL);
			shader.use();
			// ... set view and projection matrix
			cubeMesh.use();
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.id);
			glDrawArrays(GL_TRIANGLES, 0, cubeMesh.vertexCount);
			glDepthFunc(GL_LESS);
		}
	};
}
