#pragma once

#include "../engine/shader.h"
#include "../engine/mesh.h"
#include "../engine/framebuffer.h"
#include "vertex.h"

namespace Electric {

	class DofShader : public Engine::Shader
	{
	private:

		typedef Engine::Mesh<TexturedVertex> Mesh;
		typedef Engine::Framebuffer Framebuffer;

		struct Pass
		{
			GLuint program;
			GLuint vao;
			GLint textureLocation;
		};

		Pass m_pass;

		static Pass setupPass(GLuint vs, GLuint ps, const Mesh& mesh);
		static void preparePass(const Pass& pass, const Framebuffer& texture, UINT index);

	public:

		DofShader()
		{}

		bool init(AAssetManager *pAssetManager, const Mesh& mesh);

		virtual void release() override {

			glDeleteProgram(m_pass.program);
			GLuint handles[] = {m_pass.vao};
			glDeleteVertexArrays(1, handles);
		}

		void render(const Mesh& mesh, const Framebuffer& framebuffer, UINT index);

		void updateSize(GLsizei width, GLsizei height)
		{
			/*
			GLsizei w = static_cast<GLsizei>( static_cast<float>(width) * 0.25f);
			GLsizei h = static_cast<GLsizei>( static_cast<float>(height) * 0.25f);
			m_framebuffer1.init(w, h, true, Framebuffer::NONE);
			m_framebuffer2.init(w, h, true, Framebuffer::NONE);
			 */
		}
	};
};
