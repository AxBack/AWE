#pragma once

#include "node_shader.h"

namespace Electric {

	class NodeOverlayShader : public NodeShader {
	private:

#define FORWARD "forward"

		GLint m_forwardLocation;

	protected:

		GLuint createProgram(AAssetManager* pAssetManager) const override
		{
			return Engine::Shader::createProgram(pAssetManager, "shaders/NodeOverlayShader_vs.glsl", "shaders/NodeOverlayShader_ps.glsl");
		}

		virtual void extractUniforms(GLuint program) override
		{
			NodeShader::extractUniforms(program);
			m_forwardLocation = getLocation(program, FORWARD);
		}

		virtual void setUniforms(const Engine::Camera& camera) override
		{
			NodeShader::setUniforms(camera);
			glUniform3fv(m_forwardLocation, 1, camera.getForward().data());
		}

	public:

		NodeOverlayShader()
		{}
	};
}
