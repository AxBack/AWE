#include "bloom_shader.h"

namespace Electric {

#define TEXTURE "uTexture"
#define OVERLAY "uOverlay"

	bool BloomShader::init(AAssetManager* pAssetManager, const Mesh& mesh)
	{
		GLuint vs = createShader(pAssetManager, "shaders/FullscreenShader_vs.glsl", GL_VERTEX_SHADER);

		{
			GLuint ps = createShader(pAssetManager, "shaders/ThresholdShader_ps.glsl", GL_FRAGMENT_SHADER);
			m_thresholdPass = setupPass(vs, ps, mesh);
			glDeleteShader(ps);
		}

		{
			GLuint ps = createShader(pAssetManager, "shaders/BlurHorizontal_ps.glsl", GL_FRAGMENT_SHADER);
			m_horizontalBlurPass = setupPass(vs, ps, mesh);
			glDeleteShader(ps);
		}

		{
			GLuint ps = createShader(pAssetManager, "shaders/BlurVertical_ps.glsl", GL_FRAGMENT_SHADER);
			m_verticalBlurPass = setupPass(vs, ps, mesh);
			glDeleteShader(ps);
		}

		{
			GLuint ps = createShader(pAssetManager, "shaders/BloomShader_ps.glsl", GL_FRAGMENT_SHADER);
			m_finalPass = setupPass(vs, ps, mesh);
			glDeleteShader(ps);
		}

		glDeleteShader(vs);

		return true;
	}

	auto BloomShader::setupPass(GLuint vs, GLuint ps, const Mesh& mesh)->Pass
	{
		Pass pass = {0,0,-1,-1};
		pass.program = createProgram(vs, ps);
		pass.textureLocation = getLocation(pass.program, TEXTURE);
		pass.overlayLocation = getLocation(pass.program, OVERLAY);

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

	void BloomShader::render(const Mesh& mesh, const Framebuffer& framebuffer)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		m_framebuffer2.set();
		m_framebuffer2.clear();

		preparePass(m_thresholdPass, framebuffer);
		mesh.render();

		m_framebuffer1.set();
		m_framebuffer1.clear();

		preparePass(m_horizontalBlurPass, m_framebuffer2);
		mesh.render();

		m_framebuffer2.set();
		m_framebuffer2.clear();

		preparePass(m_verticalBlurPass, m_framebuffer1);
		mesh.render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0,0,framebuffer.getWidth(), framebuffer.getHeight());

		preparePass(m_finalPass, framebuffer, m_framebuffer2);
		mesh.render();
	}

	void BloomShader::preparePass(const Pass& pass, const Framebuffer& texture)
	{
		glUseProgram(pass.program);
		glBindVertexArray(pass.vao);

		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		glUniform1i(pass.textureLocation, 0);
	}

	void BloomShader::preparePass(const Pass& pass, const Framebuffer& texture, const Framebuffer& overlay)
	{
		preparePass(pass, texture);
		glActiveTexture(GL_TEXTURE1);
		overlay.bind();
		glUniform1i(pass.overlayLocation, 1);
	}
}