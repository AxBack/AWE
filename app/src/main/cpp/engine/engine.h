#pragma once

#include <android/asset_manager_jni.h>

#include <vector>
#include "matrix.h"
#include "../pch.h"

#include "updater.h"
#include "mesh.h"


class Engine
{
protected:

	static UINT sThreadCounter;

	UINT				m_id;

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

	virtual void clear(){};

	virtual bool init(AAssetManager* pAssetManager) = 0;

	virtual bool render() = 0;

	virtual void resume() = 0;
	virtual void pause() = 0;

	virtual void updateSize(int width, int height) = 0;
	virtual void touch(float /*x*/, float /*y*/) {}
};

