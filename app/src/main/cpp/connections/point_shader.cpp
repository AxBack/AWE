#include "point_shader.h"

namespace Connections {

	bool PointShader::init(AAssetManager *pAssetManager, const Engine::IMesh &mesh) {

		m_program = createProgram(pAssetManager, "shaders/PointShader_vs.glsl", "shaders/Simple_ps.glsl");
		m_viewProjectionLocation = glGetUniformLocation(m_program, VIEW_PROJECTION);

		bindTo(mesh);

		return true;
	}

	void PointShader::bindTo(const Engine::IMesh &mesh) {
		if (m_vao <= 0)
			glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PointInstance), 0);
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(PointInstance), BUFFER_OFFSET(3));
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(PointInstance), BUFFER_OFFSET(4));
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);
	}
}