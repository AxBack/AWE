#pragma once

#include "engine/shader.h"
#include "engine/matrix.h"
#include "vertex.h"
#include "engine/mesh.h"

class PointShader : public Shader
{
private:

#define VIEW_PROJECTION "viewProjection"

	GLint m_viewProjectionLocation;

public:

	bool init(AAssetManager* pAssetManager, const IMesh& mesh);
	void bindTo(const IMesh& mesh);

	void bind(const Matrix& viewProjection)
	{
		glUseProgram(m_program);
		glBindVertexArray(m_vao);
		glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, viewProjection.data());
	}
};
