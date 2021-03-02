#pragma once

#include <glad/glad.h> 
#include <vector>

namespace engine {

	using namespace std;

	struct Color {
		float r;
		float g;
		float b;
		float a;


		/// <summary>
		/// create color from rgba values 0.0-1.0
		/// </summary>
		inline static Color rgba(float r, float g, float b, float a) {
			return Color{ r, g, b, a };
		}

		/// <summary>
		/// create color from rgb values 0.0-1.0
		/// </summary>
		inline static Color rgb(float r, float g, float b) {
			return rgba(r, g, b, 1);
		}

		/// <summary>
		/// create color from rgb values 0-255
		/// </summary>
		inline static Color rgb(int r, int g, int b) {
			return rgba((float)r / 255, (float)g / 255, (float)b / 255, 1);
		}

		static Color red;
		static Color green;
		static Color blue;
		static Color cyan;
		static Color yellow;
		static Color magenta;
		static Color clear;
	};


	Color Color::red = Color{ .87f,.10f,.19f,1 };
	Color Color::green = Color{ .10f,.87f,.29f,1 };
	Color Color::blue = Color{ .10f,.19f,.87f,1 };
	Color Color::cyan = Color{ .10f,.87f,.84f,1 };
	Color Color::yellow = Color{ .92f,.81f,.35f,1 };
	Color Color::magenta = Color{ .64f,.20f,.81f,1 };
	Color Color::clear = Color{ 0,0,0,0 };

	// records gizmo calls as a vertexbuffer+color+fill mode (probably? might have to change)
	// after rendering the main geometry the gizmos are drawn by looping over the collected calls
	// then clear the buffer at the end of the frame

	// TODO: initialize current color to white or something..

	struct GizmoRequest {
		Color color;
		int fillmode;
	};

	class Gizmos {
	public:

		static GLuint VAO;
		static GLuint VBO;

		static vector<GizmoRequest> requests;
		static vector<float> vertices;

		static Color current;
		static void SetColor(Color c) {
			current = c;
		}

		static void line(float x, float y, float x2, float y2) {
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(0);
			vertices.push_back(x2);
			vertices.push_back(y2);
			vertices.push_back(0);
			GizmoRequest req{
				current,
				GL_LINE
			};
			requests.push_back(req);
		}

		static void clear() { requests.clear(); }

		static void init() {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			int drawMode = GL_DYNAMIC_DRAW;

			// ===

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], drawMode);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 0, 0, drawMode);


			// ===
			int posAttribute = 0;
			// index, size, type, normalized, stride, offset
			glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(posAttribute);

		}
		static void update_mesh() {

			int drawMode = GL_DYNAMIC_DRAW;

			// ===

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), &vertices[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], drawMode);
		}
	};

	GLuint Gizmos::VBO;
	GLuint Gizmos::VAO;
	Color Gizmos::current = Color::red;
	vector<GizmoRequest> Gizmos::requests;
	vector<float> Gizmos::vertices;
}
