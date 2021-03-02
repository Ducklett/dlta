#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

namespace engine {
	using namespace std;

	class Shader {
	public:
		GLuint ID;

		Shader() {
			ID = 0;
		}

		Shader(const char* vertexPath, const char* fragmentPath) {
			string vSource = source_from_file(vertexPath);
			string fSource = source_from_file(fragmentPath);
			char infoLog[512];

			GLuint vertexShader = makeShader(GL_VERTEX_SHADER, vSource.c_str());
			GLuint fragmentShader = makeShader(GL_FRAGMENT_SHADER, fSource.c_str());

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

			ID = program;
		}

		void use() { glUseProgram(ID); }
		void setBool(const string& name, bool value) const { return glUniform1i(uniformLocation(name), (int)value); }
		void setInt(const string& name, int value) const { return glUniform1i(uniformLocation(name), value); }
		void setFloat(const string& name, float value) const { return glUniform1f(uniformLocation(name), value); }
		void setVec2(const string& name, float x, float y) const { return glUniform2f(uniformLocation(name), x, y); }
		void setVec3(const string& name, float x, float y, float z) const { return glUniform3f(uniformLocation(name), x, y, z); }
		void setVec4(const string& name, float x, float y, float z, float w) const { return glUniform4f(uniformLocation(name), x, y, z, w); }

	private:
		static string source_from_file(const char* path) {
			string code;
			ifstream file;
			file.exceptions(ifstream::failbit | ifstream::badbit);
			try {

				file.open(path);
				stringstream stream;
				stream << file.rdbuf();
				file.close();
				code = stream.str();
			}
			catch (ifstream::failure e) {
				cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
			}
			return move(code);
		}

		static GLuint makeShader(int type, const GLchar* const source) {

			char infoLog[512];

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

		GLuint uniformLocation(const string& name) const {
			// TODO: consider chaching uniform location lookup
			return glGetUniformLocation(ID, name.c_str());
		}
	};

}
