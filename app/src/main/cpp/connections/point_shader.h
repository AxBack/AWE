#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"

namespace Connections {

	class PointShader : public Engine::Shader {
	private:

		typedef Engine::InstancedMesh<Vertex, PointInstance> Mesh;

#define VIEW_PROJECTION "viewProjection"

		GLuint m_program;
		GLuint m_vao;
		GLint m_viewProjectionLocation;

	public:

		PointShader()
				: m_program(0)
				, m_vao(0)
				, m_viewProjectionLocation(0)
		{}

		bool init(AAssetManager *pAssetManager, const Mesh &mesh);

		void bindTo(const Mesh &mesh);

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

		void render(const Engine::Camera& camera, const Mesh& mesh)
		{
			glUseProgram(m_program);
			glBindVertexArray(m_vao);
			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, camera.getViewProjection().data());
			mesh.render();
		}
	};
}