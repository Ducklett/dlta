#pragma once

#include <glad/glad.h> 
#include <vector>
#include "vec.h"
#include "Shader.h"


namespace engine {

	using namespace std;
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

		static Shader shader;
		static GLuint VAO;
		static GLuint VBO;

		static vector<GizmoRequest> requests;
		static vector<float> vertices;

		static Color current;
		static void SetColor(Color c) {
			current = c;
		}

		static void line(float x, float y, float x2, float y2) {
			vertices.insert(vertices.end(), { x,y,0,x2, y2, 0 });
			GizmoRequest req{
				current,
				GL_LINES
			};
			requests.push_back(req);
		}

		static void clear() {
			requests.clear();
			vertices.clear();
		}

		static void init() {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			int drawMode = GL_DYNAMIC_DRAW;

			// ===

			int MAX_VERTS = 0xFFFF * 3;

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], drawMode);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MAX_VERTS, 0, drawMode);


			// ===
			int posAttribute = 0;
			// index, size, type, normalized, stride, offset
			glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(posAttribute);

		}
		static void update_mesh() {

			glBindVertexArray(Gizmos::VAO);

			int drawMode = GL_DYNAMIC_DRAW;

			// ===

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//              target        offset    size                           data
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), &vertices[0]);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], drawMode);
		}
		static void draw() {
			Gizmos::update_mesh();
			shader.use();

			for (int i = 0; i < Gizmos::requests.size(); i++) {
				int start = i * 2;
				shader.setVec3("color", Gizmos::requests[i].color);
				glDrawArrays(GL_LINES, start, 2);
			}
			Gizmos::clear();
		}
	};

	Shader Gizmos::shader;
	GLuint Gizmos::VBO;
	GLuint Gizmos::VAO;
	Color Gizmos::current = Color::red;
	vector<GizmoRequest> Gizmos::requests;
	vector<float> Gizmos::vertices;
}
