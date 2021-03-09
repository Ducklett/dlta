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
		GLuint quad;
		GLuint cube;
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

			quad = makeQuad();
			cube = makeCube();

			errorShader = Shader("assets/error.vert", "assets/error.frag");
			testShader = Shader("assets/test.vert", "assets/test.frag");
			testShader2 = Shader("assets/test2.vert", "assets/test2.frag");

			Gizmos::init();
			Gizmos::shader = Shader("assets/gizmo.vert", "assets/gizmo.frag");
			tex = Texture::load("assets/container.jpg");
			tex2 = Texture::load("assets/icon.png", false, false, false);

			tex.bind(0);
			tex2.bind(1);

			// Initialize input
			// this ensures the mouse delta becomes zero on the first frame
			Input::Update(window, vec2(width, height));
		}

		GLuint makeQuad() {
			const float vertices[] = {
				// position         color                uv
				0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
				0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
			};

			const unsigned int indices[] = {  // note that we start from 0!
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};

			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			int drawMode = GL_STATIC_DRAW;

			// ===
			GLuint VBO;

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, drawMode);

			// ===
			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, drawMode);


			// ===
			int posAttribute = 0;
			// index, size, type, normalized, stride, offset
			glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(posAttribute);

			int colAttribute = 1;
			glVertexAttribPointer(colAttribute, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(colAttribute);

			int uvAttribute = 2;
			glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(uvAttribute);

			return VAO;
		}

		GLuint makeCube() {
			float vertices[] = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0,0,-1,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0,0,-1,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0,0,-1,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0,0,-1,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0,0,-1,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0,0,-1,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0,0,1,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0,0,1,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0,0,1,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0,0,1,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0,0,1,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0,0,1,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1,0,0,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1,0,0,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1,0,0,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1,0,0,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1,0,0,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1,0,0,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,0,0,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1,0,0,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1,0,0,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1,0,0,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1,0,0,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,0,0,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,-1,0,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0,-1,0,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0,-1,0,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0,-1,0,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0,-1,0,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,-1,0,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0,1,0,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0,1,0,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0,1,0,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0,1,0,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0,1,0,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0,1,0
			};

			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			int drawMode = GL_STATIC_DRAW;

			// ===
			GLuint VBO;

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, drawMode);

			// ===
			int posAttribute = 0;
			// index, size, type, normalized, stride, offset
			glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(posAttribute);

			int uvAttribute = 1;
			glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(uvAttribute);

			int normalAttribute = 2;
			glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(normalAttribute);

			return VAO;
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
				glBindVertexArray(rndr->mesh);
				Shader& shader = rndr->shader;
				shader.use(true);
				// TODO: move this into material
				shader.setTexture("tex", tex);
				shader.setTexture("tex2", tex2);

				mat4 model = glm::mat4(1);
				model = glm::translate(model, rndr->transform.position);
				// TODO: apply rotation
				model = glm::rotate(model, rndr->transform.euler.x, vec3(1, 0, 0));
				model = glm::rotate(model, rndr->transform.euler.y, vec3(0, 1, 0));
				model = glm::rotate(model, rndr->transform.euler.z, vec3(0, 0, 1));
				model = glm::scale(model, rndr->transform.scale);
				testShader.setMat4("model", model);
				// TODO: store vert count in mesh, retrieve from renderer
				glDrawArrays(GL_TRIANGLES, 0 /*first*/, 36 /*vertcount*/);
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
