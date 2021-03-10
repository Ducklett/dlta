#pragma once

#include <glad/glad.h> 
#include <vector>
#include "Color.h"
#include "Shader.h"

namespace engine {

	using namespace std;

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

		static inline void line(vec2 p1, vec2 p2) { return line(vec3(p1.x, p1.y, 0), vec3(p2.x, p2.y, 0)); }
		static void line(vec3 p1, vec3 p2) {
			vertices.insert(vertices.end(), { p1.x,p1.y,p1.z,p2.x, p2.y, p2.z });
			GizmoRequest req{
				current,
				GL_LINES,
				2
			};
			requests.push_back(req);
		}

		static inline void wireQuad(vec2 p, vec2 s) { return wireQuad(p.x, p.y, 0, s.x, s.y); }
		static inline void wireQuad(vec3 p, vec2 s) { return wireQuad(p.x, p.y, p.z, s.x, s.y); }
		static void wireQuad(float x, float y, float z, float w, float h) {
			float top = y + h / 2;
			float bottom = y - h / 2;
			float left = x - w / 2;
			float right = x + w / 2;
			vertices.insert(vertices.end(), {
				left, top, z,
				right, top, z,
				right, bottom, z,
				left, bottom, z,
				});
			GizmoRequest req{
				current,
				GL_LINE_LOOP,
				4
			};
			requests.push_back(req);
		}

		static inline void quad(vec2 p, vec2 s) { return quad(p.x, p.y, 0, s.x, s.y); }
		static inline void quad(vec3 p, vec2 s) { return quad(p.x, p.y, p.z, s.x, s.y); }
		static void quad(float x, float y, float z, float w, float h) {
			float top = y + h / 2;
			float bottom = y - h / 2;
			float left = x - w / 2;
			float right = x + w / 2;
			vertices.insert(vertices.end(), {
				left, top, z,
				right, top, z,
				right, bottom, z,

				left, bottom, z,
				left, top, z,
				right, bottom, z,
				});
			GizmoRequest req{
				current,
				GL_TRIANGLES,
				6
			};
			requests.push_back(req);
		}

		static inline void wireCircle(vec2 p, float r) { return wireCircle(p.x, p.y, 0, r); }
		static inline void wireCircle(vec3 p, float r) { return wireCircle(p.x, p.y, p.z, r); }
		static void wireCircle(float x, float y, float z, float r, int axis = 0) {
			int resolution = (floor(300 * r));
			if (resolution > 100) resolution = 100;

			float off = M_PI * 2 / resolution;

			for (int i = 0; i < resolution; i++) {
				if (axis == 0) { // xy
					vertices.insert(vertices.end(), { x + sin(off * i) * r / 2,y + cos(off * i) * r / 2,z });
				}
				else if (axis == 1) { // xz
					vertices.insert(vertices.end(), { x + sin(off * i) * r / 2, y, z + cos(off * i) * r / 2 });
				}
				else { // yz
					vertices.insert(vertices.end(), { x, y + sin(off * i) * r / 2, z + cos(off * i) * r / 2 });
				}
			}

			GizmoRequest req{
				current,
				GL_LINE_LOOP,
				resolution
			};
			requests.push_back(req);
		}

		static void wireSphere(vec3 p, float r) {
			wireCircle(p.x, p.y, p.z, r);
			wireCircle(p.x, p.y, p.z, r, 1);
			wireCircle(p.x, p.y, p.z, r, 2);
		}

		static void wireCube(vec3 p, vec3 b) {
			wireQuad(p + vec3(0, 0, b.z * .5f), vec2(b.x, b.y));
			wireQuad(p - vec3(0, 0, b.z * .5f), vec2(b.x, b.y));
			line(p + vec3(b.x, b.y, b.z) * .5f, p + vec3(b.x, b.y, -b.z) * .5f);
			line(p + vec3(b.x, -b.y, b.z) * .5f, p + vec3(b.x, -b.y, -b.z) * .5f);
			line(p + vec3(-b.x, b.y, b.z) * .5f, p + vec3(-b.x, b.y, -b.z) * .5f);
			line(p + vec3(-b.x, -b.y, b.z) * .5f, p + vec3(-b.x, -b.y, -b.z) * .5f);
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
			if (requests.empty()) return;
			update_mesh();
			shader.use();

			glDisable(GL_DEPTH_TEST);

			int vertOffset = 0;
			mat4 model = mat4(1);
			shader.setMat4("model", model);
			for (auto& g : requests) {
				shader.setVec3("color", g.color);
				glDrawArrays(g.fillmode, vertOffset, g.vertCount);
				vertOffset += g.vertCount;
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
