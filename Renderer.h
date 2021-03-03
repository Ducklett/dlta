#pragma once

#include <iostream>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "Shader.h"
#include "Gizmos.h"

namespace engine {
	void resize_framebuffer(GLFWwindow* window, int width, int height);

	class Renderer {
	public:
		GLFWwindow* window;
		GLuint quad;
		Shader errorShader;
		Shader testShader;
		int width;
		int height;

		Renderer(int width, int height, const std::string& title = "engine") {
			this->width = width;
			this->height = height;

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

			// lock the framerate to screen refresh rate
			glfwSwapInterval(1);

			glfwSetWindowUserPointer(window, this);

			glfwSetFramebufferSizeCallback(window, resize_framebuffer);

			quad = makeQuad();

			errorShader = Shader("assets/error.vert", "assets/error.frag");
			testShader = Shader("assets/test.vert", "assets/test.frag");

			Gizmos::init();
			Gizmos::shader = Shader("assets/gizmo.vert", "assets/gizmo.frag");
		}

		GLuint makeQuad() {
			const float vertices[] = {
				// position         color
				//0.5f,  0.5f, 0.0f,  1,0,0,  // top right
				0.0f,  0.5f, 0.0f,  1,0,0,  // top right
				0.5f, -0.5f, 0.0f,  0,1,0,   // bottom right
				-0.5f, -0.5f, 0.0f, 0,0,1,  // bottom left
				//-0.5f,  0.5f, 0.0f   // top left 
			};

			const unsigned int indices[] = {  // note that we start from 0!
				0, 1, 2,   // first triangle
				//1, 2, 3    // second triangle
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
			glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(posAttribute);

			int colAttribute = 1;
			glVertexAttribPointer(colAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(colAttribute);

			return VAO;
		}


		void run() {
			while (!glfwWindowShouldClose(window)) {

				process_input();

				render();
			}

			glfwTerminate();
		}

		bool wire = false;

		float rand01() {
			return ((float)rand()) / (float)RAND_MAX;
		}

		void process_input() {

			// draw cursor
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			xpos = (xpos / width) * 2 - 1;
			ypos = (1 - (ypos / height)) * 2 - 1;

			Gizmos::SetColor(Color::red);
			Gizmos::line(0, 0, xpos, ypos);
			// ===

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}

			wire = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

			float greenValue = (sin(glfwGetTime()) * .5f + .5f);
			testShader.setVec4("color", 0, greenValue, 0, 1);

			//glUniform4f(colorLocation, 0.0f, greenValue, 0.0f, 1.0f);
			float t = glfwGetTime();
			Gizmos::SetColor(Color::rgb(
				(float)(sin(t) * .5 + .5),
				sin(t + 1.48) * .5 + .5,
				sin(t + 3.456) * .5 + .5));

			Gizmos::line(0, 0, cos(glfwGetTime()), sin(glfwGetTime()));
		}

		void render() {

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			testShader.use();
			glBindVertexArray(quad);

			// render experiments:
			//glDrawArrays(GL_TRIANGLES, 0 /*first*/, 3 /*vertcount*/);
			//glDrawArrays(GL_POINTS, 0 /*first*/, 4 /*vertcount*/);
			//glPolygonMode(GL_FRONT_AND_BACK,  GL_LINE);
			//glPolygonMode(GL_FRONT_AND_BACK,  GL_FILL);

			if (wire) {
				// pretty sure this won't work for other meshes lmao
				// but for the quad it's good!
				glDrawArrays(GL_LINE_LOOP, 0 /*first*/, 3 /*vertcount*/);
			}
			else {
				glDrawElements(GL_TRIANGLES, 3 /*count*/, GL_UNSIGNED_INT, 0);
			}

			// draw gizmos
			Gizmos::draw();

			// swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		static void panic(const std::string& msg) {

			std::cout << msg << std::endl;
			exit(-1);
		}
	};

	void resize_framebuffer(GLFWwindow* window, int width, int height) {
		Renderer* r = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		r->width = width;
		r->height = height;
		glViewport(0, 0, width, height);
		// noticed this allocates a ton of ram...
		//r->render();
	}
}
