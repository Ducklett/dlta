#pragma once

#include <glad/glad.h> 
#include <vector>
#include <cmath>
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
		int vertCount;
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
				GL_LINES,
				2
			};
			requests.push_back(req);
		}

		static void wireQuad(float x, float y, float w, float h) {
			float top = y + h / 2;
			float bottom = y - h / 2;
			float left = x - w / 2;
			float right = x + w / 2;
			vertices.insert(vertices.end(), {
				left, top, 0,
				right, top, 0,
				right, bottom, 0,
				left, bottom, 0,
				});
			GizmoRequest req{
				current,
				GL_LINE_LOOP,
				4
			};
			requests.push_back(req);
		}

		static void wireCircle(float x, float y, float r) {
			int resolution = (floor(300 * r));
			if (resolution > 100) resolution = 100;

			float off = M_PI * 2 / resolution;

			for (int i = 0; i < resolution; i++) {
				vertices.insert(vertices.end(), { x + sin(off * i) * r / 2,y + cos(off * i) * r / 2,0 });
			}

			GizmoRequest req{
				current,
				GL_LINE_LOOP,
				resolution
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

			// TODO: detect when we reach the max vertex count and do something about it.
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

			int vertOffset = 0;
			for (auto& g : Gizmos::requests) {
				shader.setVec3("color", g.color);
				glDrawArrays(g.fillmode, vertOffset, g.vertCount);
				vertOffset += g.vertCount;
			}
			for (int i = 0; i < Gizmos::requests.size(); i++) {
				int start = i * 2;
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
