#include "dof_shader.h"

namespace Electric {

#define TEXTURE "uTexture"

	bool DofShader::init(AAssetManager* pAssetManager, const Mesh& mesh)
	{
		GLuint vs = createShader(pAssetManager, "shaders/FullscreenShader_vs.glsl", GL_VERTEX_SHADER);

		{
			GLuint ps = createShader(pAssetManager, "shaders/TexturedShader_ps.glsl", GL_FRAGMENT_SHADER);
			m_pass = setupPass(vs, ps, mesh);
			glDeleteShader(ps);
		}

		glDeleteShader(vs);

		return true;
	}

	auto DofShader::setupPass(GLuint vs, GLuint ps, const Mesh& mesh)->Pass
	{
		Pass pass = {0,0,-1};
		pass.program = createProgram(vs, ps);
		pass.textureLocation = getLocation(pass.program, TEXTURE);

		glGenVertexArrays(1, &pass.vao);
		glBindVertexArray(pass.vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), BUFFER_OFFSET(3));

		glBindVertexArray(0);
		return pass;
	}

	void DofShader::render(const Mesh& mesh, const Framebuffer& framebuffer, UINT index)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		preparePass(m_pass, framebuffer, index);
		mesh.render();
	}

	void DofShader::preparePass(const Pass& pass, const Framebuffer& texture, UINT index)
	{
		glUseProgram(pass.program);
		glBindVertexArray(pass.vao);

		glActiveTexture(GL_TEXTURE0);
		texture.bind(index);
		glUniform1i(pass.textureLocation, 0);
	}
}