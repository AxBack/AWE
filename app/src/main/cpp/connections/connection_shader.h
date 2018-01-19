#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"

namespace Connections {

	class ConnectionShader : public Engine::Shader {
	private:

#define VIEW_PROJECTION "viewProjection"

		GLint m_viewProjectionLocation;

	public:

		bool init(AAssetManager *pAssetManager, const Engine::InstancedMesh<Vertex, ConnectionInstance>& mesh);

		void bindTo(const Engine::InstancedMesh<Vertex, ConnectionInstance>& mesh);


		void render(const Engine::Camera& camera, const Engine::InstancedMesh<Vertex, ConnectionInstance>& mesh) {
			glUseProgram(m_program);
			glBindVertexArray(m_vao);
			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, camera.getViewProjection().data());
			mesh.render();
		}
	};
}