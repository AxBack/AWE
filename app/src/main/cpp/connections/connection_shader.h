#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include "../engine/mesh.h"

namespace Connections {

	class ConnectionShader : public Engine::Shader {
	private:

#define VIEW_PROJECTION "viewProjection"

		GLint m_viewProjectionLocation;

	public:

		bool init(AAssetManager *pAssetManager, const Engine::IMesh &mesh);

		void bindTo(const Engine::IMesh &mesh);


		void bind(const Math::Matrix &viewProjection) {
			glUseProgram(m_program);
			glBindVertexArray(m_vao);
			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, viewProjection.data());
		}
	};
}