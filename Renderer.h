#pragma once

#include <iostream>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>

char infoLog[512];

void resize_framebuffer(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

class Renderer {
public:
	GLFWwindow* window;
	GLuint triange;
	GLuint shaderProgram;

	Renderer(int width, int height, const std::string& title = "engine") {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// macOS
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (window == NULL) {
			glfwTerminate();
			panic("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) panic("Failed to initialize GLAD");

		glViewport(0, 0, width, height);

		glfwSetFramebufferSizeCallback(window, resize_framebuffer);

		triange = makeTriange();

		shaderProgram = makeProgram();

	}

	GLuint makeTriange() {
		const float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};

		GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// === vbo

		GLuint VBO;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// copy verts into the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// === attributes

		int posAttribute = 0;
		// index, size, type, normalized, stride, offset
		glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(posAttribute);

		return VAO;
	}

	GLuint makeProgram() {
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		const char* fragmentShaderSource = "#version 330 core\n" \
			"out vec4 FragColor;\n"\
			"void main()\n"\
			"{\n"\
			"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"\
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
			// input
			process_input(window);

			// rendering
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// will move to update loop
			glUseProgram(shaderProgram);
			glBindVertexArray(triange);
			glDrawArrays(GL_TRIANGLES, 0 /*first*/, 3 /*vertcount*/);

			// swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}

	static void panic(const std::string& msg) {

		std::cout << msg << std::endl;
		exit(-1);
	}
};
