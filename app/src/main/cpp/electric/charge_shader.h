#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"

namespace Electric {

	class ChargeShader : public Engine::Shader {
	private:

		GLint m_viewProjectionLocation;

	public:

		bool init(AAssetManager *pAssetManager, const Engine::IMesh &mesh);

		void bindTo(const Engine::IMesh &mesh);

		void bind(const Engine::Camera& camera)
		{
			glUseProgram(m_program);
			glBindVertexArray(m_vao);
			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, camera.getViewProjection().data());
		}
	};
}
