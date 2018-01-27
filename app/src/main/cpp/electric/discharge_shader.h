#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"
#include "vertex.h"
#include "../engine/texture.h"

namespace Electric {

	class DischargeShader : public Engine::Shader {
	private:

		typedef Engine::InstancedMesh<DischargeVertex, DischargeInstance> Mesh;

		GLuint m_program;
		GLuint m_vao;
		GLint m_viewProjectionLocation;
		GLint m_textureLocation;

	public:

		DischargeShader()
				: m_program(0)
				, m_vao(0)
				, m_viewProjectionLocation(0)
		{}

		bool init(AAssetManager *pAssetManager, const Mesh& mesh);

		virtual void release() override {
			if (m_program > 0) {
				glDeleteProgram(m_program);
				m_program = 0;
			}

			if (m_vao > 0) {
				glDeleteVertexArrays(1, &m_vao);
				m_vao = 0;
			}
		}

		void render(const Engine::Camera& camera, const Mesh& mesh, Engine::Texture& texture )
		{
			glUseProgram(m_program);
			glBindVertexArray(m_vao);

			glActiveTexture(GL_TEXTURE0);
			texture.bind();
			glUniform1i(m_textureLocation, 0);

			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, camera.getViewProjection().data());

			mesh.render();
		}
	};
}
