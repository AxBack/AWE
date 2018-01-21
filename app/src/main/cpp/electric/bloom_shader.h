#pragma once

#include "../engine/shader.h"
#include "../engine/mesh.h"
#include "../engine/framebuffer.h"
#include "vertex.h"

namespace Electric {

	class BloomShader : public Engine::Shader
	{
	private:

		typedef Engine::Mesh<TexturedVertex> Mesh;
		typedef Engine::Framebuffer Framebuffer;

		struct Pass
		{
			GLuint program;
			GLuint vao;
			GLint textureLocation;
			GLint overlayLocation;
		};

		Pass m_thresholdPass;

		Pass m_horizontalBlurPass;

		Pass m_verticalBlurPass;

		Pass m_finalPass;

		Framebuffer m_framebuffer1;
		Framebuffer m_framebuffer2;

		static Pass setupPass(GLuint vs, GLuint ps, const Mesh& mesh);
		static void preparePass(const Pass& pass, const Framebuffer& texture);
		static void preparePass(const Pass& pass, const Framebuffer& texture, const Framebuffer& overlay);

	public:

		BloomShader()
		{}

		bool init(AAssetManager *pAssetManager, const Mesh& mesh);

		virtual void release() override {

			glDeleteProgram(m_thresholdPass.program);
			glDeleteProgram(m_horizontalBlurPass.program);
			glDeleteProgram(m_verticalBlurPass.program);
			glDeleteProgram(m_finalPass.program);

			GLuint handles[] = {m_thresholdPass.vao,
								m_horizontalBlurPass.vao,
								m_verticalBlurPass.vao,
								m_finalPass.vao};
			glDeleteVertexArrays(4, handles);

			m_framebuffer2.release();
			m_framebuffer1.release();
		}

		void render(const Mesh& mesh, const Framebuffer& framebuffer);

		void updateSize(GLsizei width, GLsizei height)
		{
			GLsizei w = static_cast<GLsizei>( static_cast<float>(width) * 0.25f);
			GLsizei h = static_cast<GLsizei>( static_cast<float>(height) * 0.25f);
			m_framebuffer1.init(w, h, true, Framebuffer::NONE);
			m_framebuffer2.init(w, h, true, Framebuffer::NONE);
		}
	};
};
