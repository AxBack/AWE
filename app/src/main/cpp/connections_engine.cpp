#include "connections_engine.h"

void ConnectionsEngine::clear()
{
	m_pointShader.release();
	m_pointMesh.clean();
	m_updater.pause();
	LOGI("Engine( Cleared: %d )", m_id);
}

bool ConnectionsEngine::init(AAssetManager* pAssetManager)
{
	clear();

	LOGI("Engine( Init begin: %d )", m_id);

	{
		Vertex vertices[] = {
				{-0.5f, 0.5f,  0, 0},
				{0.5f,  0.5f,  0, 0},
				{0.5f,  -0.5f, 0, 0},
				{-0.5f, -0.5f, 0, 0}
		};

		GLushort indices[] = {0, 1, 2, 0, 2, 3};
		if(!m_pointMesh.init(4, vertices, 6, indices))
		{
			LOGD("init( Failed to init PointMesh: %d )", m_id);
			return false;
		}

		if(!m_pointShader.init(pAssetManager, m_pointMesh))
		{
			LOGD("init( Failed to init PointShader: %d )", m_id);
			return false;
		}
	}

	{
		Vertex vertices[] = {
				{-1.0f, 0.0f,  0, 0},
				{1.0f,  0.0f,  0, 0},
				{-1.0f, 0.25f,  0, 0.7f},
				{1.0f,  0.25f,  0, 0.7f},
				{-1.0f, 0.5f,  0, 1},
				{1.0f,  0.5f,  0, 1},
				{-1.0f, 0.75f,  0, 0.7f},
				{1.0f,  0.75f,  0, 0.7f},
				{-1.0f, 1.0f, 0, 0},
				{1.0f,  1.0f, 0, 0}
		};

		GLushort indices[] = {
				0, 1, 3,
				0, 3, 2,
				2, 3, 5,
				2, 5, 4,
				4, 5, 7,
				4, 7, 6,
				6, 7, 9,
				6, 9, 8,
		};
		if(!m_connectionMesh.init(10, vertices, 24, indices))
		{
			LOGD("init( Failed to init ConnectionMesh: %d )", m_id);
			return false;
		}

		if(!m_connectionShader.init(pAssetManager, m_connectionMesh))
		{
			LOGD("init( Failed to init ConnectionShader: %d )", m_id);
			return false;
		}
	}
	updateSize(1,1);

	if(!m_updater.init())
	{
		LOGD("init( Failed to init updater: %d )", m_id);
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	LOGI("Engine( Init end: %d )", m_id);

	return true;
}

bool ConnectionsEngine::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	if(!m_updater.updateInstances(m_pointMesh, m_connectionMesh))
		return false;

	m_connectionShader.bind(m_viewProjection);
	m_connectionMesh.render();

	m_pointShader.bind(m_viewProjection);
	m_pointMesh.render();

	return true;
}

void ConnectionsEngine::updateSize(int width, int height)
{
	LOGI("Engine(size updated: %d, %d )", width, height);
	m_viewport[0] = m_viewport[1] = 0;
	m_viewport[2] = width;
	m_viewport[3] = height;

	Matrix projection, view;

    Matrix::perspective(projection, 90.0f, static_cast<float>(width) / static_cast<float>(height), 10, 110);
	Matrix::lookAt(view, {0, 0, -10.0f}, {0, 0, 1}, {0, 1, 0});
	m_viewProjection = projection * view;
	Matrix::invert(m_viewProjection, m_inverseViewProjection);
}

void ConnectionsEngine::touch(float x, float y)
{
	Vector3 start = {x - m_viewport[2] * 0.5f, y - m_viewport[3] * 0.5f, 0.0f};
	Vector3 end = m_inverseViewProjection.project(start);
	m_updater.rayTest(start, end);
}