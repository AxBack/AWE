#pragma once

#include <android/asset_manager_jni.h>

#include <vector>
#include "matrix.h"
#include "pch.h"

#include "item_shader.h"
#include "updater.h"
#include "mesh.h"
#include "vertex.h"


class Engine
{
private:

	static UINT sThreadCounter;

	UINT					m_id;

	Updater					m_updater;

	ItemShader 				m_itemShader;
	Mesh<Vertex, Instance> 	m_mesh;

	Matrix          		m_viewProjection;
	GLint					m_viewport[4];

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

