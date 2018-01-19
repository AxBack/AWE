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

		GLint m_viewProjectionLocation;
		GLint m_rightLocation;
		GLint m_upLocation;

	public:

		bool init(AAssetManager *pAssetManager, const Mesh &mesh);

		void bindTo(const Mesh &mesh);

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
