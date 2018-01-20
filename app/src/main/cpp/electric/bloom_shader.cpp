#include "bloom_shader.h"

namespace Electric {

#define TEXTURE "uTexture"

	bool BloomShader::init(AAssetManager* pAssetManager, const Mesh& mesh)
	{

		GLuint vs = createShader(pAssetManager, "shaders/FullscreenShader_vs.glsl", GL_VERTEX_SHADER);

		//temp
		GLuint ps = createShader(pAssetManager, "shaders/TexturedShader_ps.glsl", GL_FRAGMENT_SHADER);


		m_pass.program = createProgram(vs, ps);
		m_pass.textureLocation = getLocation(m_pass.program, TEXTURE);

		glGenVertexArrays(1, &m_pass.vao);
		glBindVertexArray(m_pass.vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), BUFFER_OFFSET(3));

		glBindVertexArray(0);

		return true;
	}

	void BloomShader::render(const Mesh& mesh, const Framebuffer& framebuffer)
	{
		glUseProgram(m_pass.program);
		glBindVertexArray(m_pass.vao);

		glActiveTexture(GL_TEXTURE0);
		framebuffer.bind();
		glUniform1i(m_pass.textureLocation, 0);

		mesh.render();
	}
}