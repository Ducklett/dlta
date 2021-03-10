#pragma once

#include "Deps.h"
#include "Shader.h"
#include "Texture.h"
#include "Gizmos.h"
#include "Input.h"
#include "Time.h"
#include "Scripting/Transform.h"
#include "Scripting/Camera.h"
#include "Scripting/Entity.h"
#include "Scripting/MeshRenderer.h"

namespace engine {
	using namespace glm;

	void resize_framebuffer(GLFWwindow* window, int width, int height);

	class Application {
	public:
		void (*update_loop)() = NULL;
		//Transform cam;
		GLFWwindow* window;
		Texture tex;
		Texture tex2;
		Shader errorShader;
		Shader testShader;
		Shader testShader2;
		vector<Entity*> entities;
		vector<MeshRenderer*> renderers;
		int width;
		int height;

		Application(int width, int height, const std::string& title = "engine") {
			this->width = width;
			this->height = height;

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

			window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

			if (window == NULL) {
				glfwTerminate();
				panic("Failed to create GLFW window");
			}

			glfwMakeContextCurrent(window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) panic("Failed to initialize GLAD");

			glViewport(0, 0, width, height);

			//glEnable(GL_MULTISAMPLE);

			// lock the framerate to screen refresh rate
			glfwSwapInterval(1);

			glfwSetWindowUserPointer(window, this);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			glfwSetFramebufferSizeCallback(window, resize_framebuffer);
			glfwSetKeyCallback(window, Input::key_change);
			glfwSetMouseButtonCallback(window, Input::mouse_change);
			glfwSetScrollCallback(window, Input::scroll_change);

			errorShader = Shader("assets/shaders/error.vert", "assets/shaders/error.frag");
			testShader = Shader("assets/shaders/test.vert", "assets/shaders/test.frag");
			testShader2 = Shader("assets/shaders/test2.vert", "assets/shaders/test2.frag");

			Gizmos::init();
			Gizmos::shader = Shader("assets/shaders/gizmo.vert", "assets/shaders/gizmo.frag");
			tex = Texture::load("assets/container.jpg");
			tex2 = Texture::load("assets/icon.png", false, false, false);

			tex.bind(0);
			tex2.bind(1);

			// Initialize input
			// this ensures the mouse delta becomes zero on the first frame
			Input::Update(window, vec2(width, height));
		}

		void run() {
			for (auto entity : entities) {
				entity->start();
			}

			while (!glfwWindowShouldClose(window)) {

				Time::Update(glfwGetTime());
				Input::Update(window, vec2(width, height));

				if (update_loop != NULL) {
					update_loop();
				}

				for (auto entity : entities) {
					entity->update();
				}

				render();
			}

			glfwTerminate();
		}

		float rand01() {
			return ((float)rand()) / (float)RAND_MAX;
		}

		void render() {
			if (Camera::main == NULL) {
				cout << "no active camera" << endl;
				return;
			}

			glEnable(GL_DEPTH_TEST);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (auto rndr : renderers) {
				rndr->mesh.use();
				Shader& shader = rndr->shader;
				shader.use(true);
				// TODO: move this into material
				shader.setTexture("tex", tex);
				shader.setTexture("tex2", tex2);

				mat4 model = glm::mat4(1);
				model = glm::translate(model, rndr->transform.position);

				model = glm::rotate(model, rndr->transform.euler.x, vec3(1, 0, 0));
				model = glm::rotate(model, rndr->transform.euler.y, vec3(0, 1, 0));
				model = glm::rotate(model, rndr->transform.euler.z, vec3(0, 0, 1));
				model = glm::scale(model, rndr->transform.scale);
				testShader.setMat4("model", model);

				rndr->mesh.draw();
			}

			// draw gizmos
			Gizmos::draw();

			// swap buffers
			glfwSwapBuffers(window);

			Input::Clear();
			glfwPollEvents();
		}

		static void panic(const std::string& msg) {

			std::cout << msg << std::endl;
			exit(-1);
		}

		void quit() {
			glfwSetWindowShouldClose(window, true);
		}
	};

	void resize_framebuffer(GLFWwindow* window, int width, int height) {
		Application* r = static_cast<Application*>(glfwGetWindowUserPointer(window));
		r->width = width;
		r->height = height;
		glViewport(0, 0, width, height);
		// noticed this allocates a ton of ram...
		//r->render();
	}
}
