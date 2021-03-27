#pragma once
#include <Dlta/Core/Shader.h>

namespace dlta {
	namespace postprocessing {
		class Effect {

		public:
			Shader shader;
			const char* title;

			Effect(const string& shaderName, const char* title) : title(title) {
				const string basePath = "assets/shaders/postprocessing/";
				const string vPath = basePath + "pp.vert";
				const string fPath = basePath + shaderName + ".frag";

				shader = Shader(vPath.c_str(), fPath.c_str());
			}

#if DLTA_EDITOR
			virtual void onEditorGui() = 0;
#endif
			virtual void onBind() = 0;
		};
	}
}
