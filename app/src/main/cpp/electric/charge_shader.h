#pragma once

#include "../engine/shader.h"
#include "../engine/matrix.h"
#include "../engine/mesh.h"
#include "../engine/camera.h"
#include "vertex.h"

namespace Electric {

	class ChargeShader : public Engine::Shader {
	private:

		typedef Engine::InstancedMesh<ChargeVertex, ChargeInstance> Mesh;

		GLint m_viewProjectionLocation;

	public:

		bool init(AAssetManager *pAssetManager, const Mesh& mesh);

		void bindTo(const Mesh& mesh);

		void render(const Engine::Camera& camera, const Mesh& mesh)
		{
			glUseProgram(m_program);
			glBindVertexArray(m_vao);
			glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, camera.getViewProjection().data());
			mesh.render();
		}
	};
}
