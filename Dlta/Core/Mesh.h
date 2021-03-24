#pragma once

#include <Dlta/Deps.h>

namespace dlta {
	using namespace std;

	enum class Vattr {
		FLOAT = 1,
		UV = 2,
		XY = 2,
		FLOAT2 = 1,
		XYZ = 3,
		RGB = 3,
		FLOAT3 = 1,
		NORMAL = 3,
		RGBA = 4,
		FLOAT4 = 4,
	};

	class Mesh {
	public:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		vector<Vattr> layout;
		vector<float> vertices;
		vector<unsigned int> indices;
		bool indexed = false;
		int vertexCount;

		// fill vertices
		// fill layout
		// call create()
		void create(size_t maxVertexCount = 0) {
			assert(!layout.empty());

			int vertexSize = 0;
			for (auto attr : layout) {
				assert((int)attr > 0);
				vertexSize += (int)attr;
			}

			if (maxVertexCount != 0) {
				vertices.reserve(maxVertexCount);
			}

			if (indexed) {
				vertexCount = indices.size();
			}
			else {
				vertexCount = vertices.size() / vertexSize;
			}

			assert(vertexCount != 0);

			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			int drawMode = GL_STATIC_DRAW;

			// ===
			GLuint VBO;

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.capacity(), &vertices[0], drawMode);

			// ===
			if (indexed) {
				unsigned int EBO;
				glGenBuffers(1, &EBO);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], drawMode);
			}

			// ===
			int attributes = layout.size();
			int offset = 0;
			for (int i = 0; i < attributes; i++) {
				uint8_t attr_size = (int)layout[i];
				// index, size, type, normalized, stride, offset
				glVertexAttribPointer(i, attr_size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(offset * sizeof(float)));
				glEnableVertexAttribArray(i);
				offset += attr_size;
			}

			this->VAO = VAO;
			this->VBO = VBO;
			this->EBO = EBO;
		}

		void update() {

		}

		void use() {
			glBindVertexArray(VAO);
		}

		void draw(int from = 0, int to = 0) {
			if (to == 0) to = vertexCount;

			if (indexed) {
				glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
			}
			else {

				glDrawArrays(GL_TRIANGLES, from, to);
			}

		}
	};
}
