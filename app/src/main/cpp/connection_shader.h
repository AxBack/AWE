#pragma once

#include <string>
#include <android/asset_manager.h>
#include "pch.h"
#include "matrix.h"
#include "vertex.h"
#include "mesh.h"

class ConnectionShader
{
private:

#define VIEW_PROJECTION "viewProjection"

	GLuint m_program;

	GLuint m_vao;

	GLint m_viewProjectionLocation;

	bool verifyShader(GLuint shader)
	{
		GLint param = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
		if(param == GL_FALSE)
		{
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &param);
			GLchar* pLog = new GLchar[param];
			glGetShaderInfoLog(shader, param, nullptr, pLog);
			LOGE("verifyShader( %s )", pLog);
			SAFE_DELETE_ARRAY(pLog);
		}
		return true;
	}

	bool loadFile(AAssetManager* pAssetManager, const std::string& asset, std::string& outFile)
	{
		AAsset* pAsset = AAssetManager_open(pAssetManager, asset.c_str(), AASSET_MODE_STREAMING);
		if(pAsset == nullptr)
		{
			LOGE("loadFile( Could not get asset: %s )", asset.c_str());
			return false;
		}

		outFile.clear();

		static const size_t size = 1024;
		int r = 0;
		char* pBuffer = new char[size];
		while((r = AAsset_read(pAsset, pBuffer, size)) > 0)
		{
			outFile.insert(outFile.end(), pBuffer, pBuffer+r);
		}
		AAsset_close(pAsset);

		SAFE_DELETE_ARRAY(pBuffer);
		return true;
	}

public:

	ConnectionShader()
			: m_program(0)
			, m_vao(0)
	{
	}

	virtual ~ConnectionShader()
	{
		release();
	}

	bool init(AAssetManager* pAssetManager, const IMesh& mesh);
	void bindTo(const IMesh& mesh);

	void release()
	{
		if(m_program > 0)
		{
			glDeleteProgram(m_program);
			m_program = 0;
		}

		if(m_vao > 0)
		{
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
	}

	void bind(const Matrix& viewProjection)
	{
		glUseProgram(m_program);
		glBindVertexArray(m_vao);
		glUniformMatrix4fv(m_viewProjectionLocation, 1, GL_FALSE, viewProjection.data());
	}
};
