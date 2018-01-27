#include "discharge_shader.h"
#include "vertex.h"

namespace Electric {

#define VIEW_PROJECTION "viewProjection"

	bool DischargeShader::init(AAssetManager* pAssetManager, const Mesh& mesh)
	{

		m_program = createProgram(pAssetManager, "shaders/DischargeShader_vs.glsl", "shaders/DischargeShader_ps.glsl");
		m_viewProjectionLocation = getLocation(m_program, VIEW_PROJECTION);

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DischargeVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(DischargeVertex), BUFFER_OFFSET(3));

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DischargeInstance), 0);
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DischargeInstance), BUFFER_OFFSET(3));
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);

		return true;
	}
}
