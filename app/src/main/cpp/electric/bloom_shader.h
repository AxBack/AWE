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
		};

		Pass m_pass;

	public:

		BloomShader()
		{}

		bool init(AAssetManager *pAssetManager, const Mesh& mesh);

		virtual void release() override {

			glDeleteProgram(m_pass.program);
			glDeleteVertexArrays(1, &m_pass.vao);
		}

		void render(const Mesh& mesh, const Framebuffer& framebuffer);
	};
};
