#include "discharge_shader.h"
#include "vertex.h"

namespace Electric {

#define VIEW_PROJECTION "viewProjection"
#define TEXTURE "uTexture"
#define CAMERA_POS "cameraPos"

	bool DischargeShader::init(AAssetManager* pAssetManager, const Mesh& mesh)
	{

		m_program = createProgram(pAssetManager, "shaders/DischargeShader_vs.glsl", "shaders/DischargeShader_ps.glsl");
		m_viewProjectionLocation = getLocation(m_program, VIEW_PROJECTION);
		m_textureLocation = getLocation(m_program, TEXTURE);
		m_cameraPosLocation = getLocation(m_program, CAMERA_POS);

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DischargeVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(DischargeVertex), BUFFER_OFFSET(3));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(DischargeVertex), BUFFER_OFFSET(4));

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DischargeInstance), 0);
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(DischargeInstance), BUFFER_OFFSET(3));
		glVertexAttribDivisor(4, 1);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(DischargeInstance), BUFFER_OFFSET(6));
		glVertexAttribDivisor(5, 1);

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(DischargeInstance), BUFFER_OFFSET(9));
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);

		return true;
	}
}
