#pragma once

#include <android/asset_manager_jni.h>

#include <vector>
#include "matrix.h"
#include "pch.h"

#include "point_shader.h"
#include "updater.h"
#include "mesh.h"
#include "vertex.h"
#include "connection_shader.h"


class Engine
{
private:

	typedef Mesh<Vertex, PointInstance> PointMesh;
	typedef Mesh<Vertex, ConnectionInstance> ConnectionMesh;

	static UINT sThreadCounter;

	UINT				m_id;

	Updater				m_updater;

	PointShader 		m_pointShader;
	PointMesh 			m_pointMesh;

	ConnectionShader 	m_connectionShader;
	ConnectionMesh 		m_connectionMesh;

	Matrix          	m_viewProjection;
	Matrix				m_inverseViewProjection;
	GLint				m_viewport[4];

public:

	Engine()
	{
		m_id = sThreadCounter;
		++sThreadCounter;
		Matrix::identity(m_viewProjection);
	}

	virtual ~Engine()
	{
		clear();
	}

	void clear();

	bool init(AAssetManager* pAssetManager);

	bool render();

	void resume() { m_updater.resume(); }
	void pause() { m_updater.pause(); }

	void updateSize(int width, int height);
	void touch(float x, float y);
};

