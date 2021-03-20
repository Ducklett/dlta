#pragma once

#include "./Deps.h"
#include "./Texture.h"

namespace dlta {
	using namespace std;

	struct FrameBuffer {
		unsigned int FBO;
		unsigned int depthStencil;
		Texture color;

		int width, height;

		FrameBuffer() {}

		FrameBuffer(int width, int height) : width(width), height(height) {
			glGenFramebuffers(1, &FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);

			// TODO: make format configurable

			// Color texture
			color = Texture::create_empty(width, height, true, false, false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.id, 0);

			cout << "framebuffer color texture" << color.id << endl;

			// Depth/stencil
			glGenRenderbuffers(1, &depthStencil);
			glBindRenderbuffer(GL_RENDERBUFFER, depthStencil);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencil);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		}

		static void unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	};
}
