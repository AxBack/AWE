#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"

namespace Electric {

	class NodeShader : public Engine::Shader {
	private:

		GLint m_viewProjectionLocation;
		GLint m_rightLocation;
		GLint m_upLocation;

	public:

		bool init(AAssetManager *pAssetManager, const Engine::IMesh &mesh);

		void bindTo(const Engine::IMesh &mesh);

		void render(const Engine::Camera& camera, const Engine::IMesh& mesh)
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
