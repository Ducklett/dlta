#pragma once

#include "Deps.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "Gizmos.h"
#include "Scripting/MeshRenderer.h"
#include "Postprocessing/Effect.h"

namespace engine {
	struct Renderer {
		FrameBuffer front;
		FrameBuffer back;
		FrameBuffer screen;

		Mesh ppQuad;

		/// <summary>
		/// When true the final pass will be rendered to the screen instead of the back buffer
		/// </summary>
		bool targetScreen = true;

		Renderer() {}
		Renderer(int width, int height, bool targetScreen = true) : targetScreen(targetScreen) {
			front = FrameBuffer(width, height);
			back = FrameBuffer(width, height);
			if (!targetScreen) screen = FrameBuffer(width, height);

			cout << "front: " << front.color.id << endl;
			cout << "back: " << back.color.id << endl;
			cout << "screen: " << screen.color.id << endl;

			ppQuad.layout = { Vattr::XY, Vattr::UV };
			ppQuad.vertices = {
				// positions   // texCoords
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			};
			ppQuad.create();
		}

		/// <summary>
		/// render the scene, the result is store in the front buffer
		/// </summary>
		void render(Skybox& skybox, const vector<MeshRenderer*>& renderers, vector<postprocessing::Effect*>& effects, Shader& testShader) {
			bool willDrawToScreen = false;

			if (effects.empty()) {
				screen.bind();
				willDrawToScreen = true;
			}
			else {
				back.bind();
			}

			//fb.bind();

			glEnable(GL_DEPTH_TEST);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// TODO: render after opaque qeometry but before transparent
			skybox.draw();

			for (auto rndr : renderers) {

				if (rndr->ignoreDepth) glDisable(GL_DEPTH_TEST);

				rndr->mesh.use();
				Shader& shader = rndr->shader;
				shader.use();
				// TODO: move this into material
				//shader.setTexture("tex", tex);
				//shader.setTexture("tex2", tex2);

				mat4 model = glm::mat4(1);
				model = glm::translate(model, rndr->transform.position);

				model = glm::rotate(model, rndr->transform.euler.x, vec3(1, 0, 0));
				model = glm::rotate(model, rndr->transform.euler.y, vec3(0, 1, 0));
				model = glm::rotate(model, rndr->transform.euler.z, vec3(0, 0, 1));
				model = glm::scale(model, rndr->transform.scale);
				shader.setMat4("model", model);

				rndr->mesh.draw();

				if (rndr->ignoreDepth) glEnable(GL_DEPTH_TEST);
			}

			// === apply postprocessing

			if (!effects.empty()) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				glDisable(GL_DEPTH_TEST);
				ppQuad.use();

				for (auto& effect : effects) {

					bool lastPass = &effect == &effects.back();


					swapBuffers();
					if (lastPass) {
						screen.bind();
						willDrawToScreen = true;
					}
					else {
						back.bind();
					}

					glClearColor(0, 0, 0, 0);
					glClear(GL_COLOR_BUFFER_BIT);

					effect->shader.use();
					effect->onBind();

					// send previous render pass to shader
					front.color.bind(0);
					//fb.color.bind(0);
					glBindTexture(GL_TEXTURE_2D, front.color.id);

					ppQuad.draw();
				}
			}

			// draw gizmos
			Gizmos::draw();

			if (!willDrawToScreen) {
				swapBuffers();
			}

			FrameBuffer::unbind();
		}

		void swapBuffers() {
			auto temp = front;
			front = back;
			back = temp;
		}

		/// <summary>
		/// returns texture with the last rendered result, use when targetScreen = false
		/// </summary>
		const Texture& getResultTexture() const {
			return screen.color;
		}
	};
}
