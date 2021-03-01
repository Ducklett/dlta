#pragma once

#include <iostream>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>

char infoLog[512];

void resize_framebuffer(GLFWwindow* window, int width, int height);

class Renderer {
public:
	GLFWwindow* window;
	GLuint quad;
	GLuint shaderProgram;
	GLuint colorLocation;

	Renderer(int width, int height, const std::string& title = "engine") {
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

		shaderProgram = makeProgram();

		colorLocation = glGetUniformLocation(shaderProgram, "color");
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

	GLuint makeProgram() {
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"out vec3 vertcolor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"   vertcolor = aColor;\n"
			"}\0";

		const char* fragmentShaderSource = "#version 330 core\n" \
			"out vec4 FragColor;\n"\
			"in vec3 vertcolor;"
			"uniform vec4 color;"
			"void main()\n"\
			"{\n"
			//"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"	FragColor = vec4(vertcolor+color.xyz, 1);\n"\
			"}\n";

		GLuint vertexShader = makeShader(GL_VERTEX_SHADER, vertexShaderSource);
		GLuint fragmentShader = makeShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		GLuint program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		int  success;
		glGetShaderiv(program, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << "PROGRAM" << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}

	GLuint makeShader(int type, const GLchar* const source) {

		GLuint shader;
		shader = glCreateShader(type);

		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		int  success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		return shader;
	}

	void run() {
		while (!glfwWindowShouldClose(window)) {
			process_input();

			render();
		}

		glfwTerminate();
	}

	bool wire = false;
	void process_input() {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		wire = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

		float greenValue = (sin(glfwGetTime()) * .5f + .5f);
		glUniform4f(colorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	}

	void render() {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// will move to update loop
		glUseProgram(shaderProgram);
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
	glViewport(0, 0, width, height);
	r->render();
}
