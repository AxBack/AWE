#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"
#include "vertex.h"

namespace Electric {

	class NodeShader : public Engine::Shader {
	private:

		typedef Engine::InstancedMesh<PositionVertex, NodeInstance> Mesh;

		GLuint m_program;
		GLuint m_vao;
		GLint m_viewProjectionLocation;
		GLint m_rightLocation;
		GLint m_upLocation;

	public:

		NodeShader()
				: m_program(0)
				, m_vao(0)
				, m_viewProjectionLocation(0)
				, m_rightLocation(0)
				, m_upLocation(0)
		{}

		bool init(AAssetManager *pAssetManager, const Mesh &mesh);

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

		void render(const Engine::Camera& camera, Mesh& mesh)
		{
			glUseProgram(m_program);
			glBindVertexArray(m_vao);
			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, camera.getViewProjection().data());
			glUniform3fv(m_upLocation, 1, camera.getUp().data());
			glUniform3fv(m_rightLocation, 1, camera.getRight().data());
			mesh.render();
		}
	};
}
