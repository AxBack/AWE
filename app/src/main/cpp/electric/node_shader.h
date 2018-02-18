#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"
#include "vertex.h"

namespace Electric {

	class NodeShader : public Engine::Shader {
	private:

#define VIEW_PROJECTION "viewProjection"
#define UP "up"
#define RIGHT "right"


		typedef Engine::InstancedMesh<PositionVertex, NodeInstance> Mesh;

		GLuint m_program;
		GLuint m_vao;
		GLint m_viewProjectionLocation;
		GLint m_rightLocation;
		GLint m_upLocation;

	protected:

		virtual GLuint createProgram(AAssetManager* pAssetManager) const
		{
			return Engine::Shader::createProgram(pAssetManager, "shaders/NodeShader_vs.glsl", "shaders/NodeShader_ps.glsl");
		}

		virtual void extractUniforms(GLuint program)
		{
			m_viewProjectionLocation = getLocation(program, VIEW_PROJECTION);
			m_upLocation = getLocation(program, UP);
			m_rightLocation = getLocation(program, RIGHT);
		}

		virtual void setUniforms(const Engine::Camera& camera)
		{
			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, camera.getViewProjection().data());
			glUniform3fv(m_upLocation, 1, camera.getUp().data());
			glUniform3fv(m_rightLocation, 1, camera.getRight().data());
		}

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
			setUniforms(camera);
			mesh.render();
		}
	};
}
