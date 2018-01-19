#include "charge_shader.h"
#include "vertex.h"

namespace Electric {

#define VIEW_PROJECTION "viewProjection"

	bool ChargeShader::init(AAssetManager* pAssetManager, const Mesh& mesh) {

		m_program = createProgram(pAssetManager, "shaders/ChargeShader_vs.glsl", "shaders/SimpleShader_ps.glsl");
		m_viewProjectionLocation = getLocation(VIEW_PROJECTION);

		bindTo(mesh);

		return true;
	}

	void ChargeShader::bindTo(const Mesh &mesh) {
		if (m_vao <= 0)
			glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChargeVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(ChargeVertex), BUFFER_OFFSET(3));

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ChargeInstance), 0);
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ChargeInstance), BUFFER_OFFSET(3));
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);
	}
}
