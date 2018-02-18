#include "node_shader.h"
#include "vertex.h"

namespace Electric {

	bool NodeShader::init(AAssetManager* pAssetManager, const Mesh& mesh)
	{

		m_program = createProgram(pAssetManager);
		extractUniforms(m_program);

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionVertex), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

		glEnableVertexAttribArray(1); //offset
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), 0);
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2); // color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), BUFFER_OFFSET(3));
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3); // size
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), BUFFER_OFFSET(6));
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(4); // size
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), BUFFER_OFFSET(7));
		glVertexAttribDivisor(4, 1);

		glBindVertexArray(0);

		return true;
	}
}
