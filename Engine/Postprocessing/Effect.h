#pragma once
#include "../Shader.h"

namespace engine {
	namespace postprocessing {
		class Effect {

		public:
			Shader shader;

			Effect(const string& shaderName) {
				const string basePath = "assets/shaders/postprocessing/";
				const string vPath = basePath + "pp.vert";
				const string fPath = basePath + shaderName + ".frag";

				shader = Shader(vPath.c_str(), fPath.c_str());
			}
		};
	}
}
