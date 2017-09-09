#include "engine.h"

UINT Engine::sThreadCounter = 0;

void Engine::clear()
{
	m_itemShader.release();
	m_mesh.clean();
	m_updater.pause();
	LOGI("Engine( Cleared: %d )", m_id);
}

bool Engine::init(AAssetManager* pAssetManager)
{
	clear();

	LOGI("Engine( Init begin: %d )", m_id);

	Vertex vertices[] = {
			{-0.5f, 0.5f,0},
			{ 0.5f, 0.5f,0},
			{ 0.5f,-0.5f,0},
			{-0.5f,-0.5f,0}
	};

	GLushort indices[] = {0,1,2,0,2,3};
	if(!m_mesh.init(4, vertices, 6, indices))
	{
		LOGD("init( Failed to init Mesh: %d )", m_id);
		return false;
	}

	if(!m_itemShader.init(pAssetManager, m_mesh))
	{
		LOGD("init( Failed to init ItemShader: %d )", m_id);
		return false;
	}

	updateSize(1,1);

	if(!m_updater.init(1.0f / 60.0f))
	{
		LOGD("init( Failed to init updater: %d )", m_id);
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	LOGI("Engine( Init end: %d )", m_id);

	return true;
}

bool Engine::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	if(!m_updater.updateInstances(m_mesh))
		return false;

	m_itemShader.bind(m_viewProjection);
	m_mesh.render();

	return true;
}

void Engine::updateSize(int width, int height)
{
	LOGI("Engine(size updated: %d, %d )", width, height);
	m_viewport[0] = m_viewport[1];
	m_viewport[2] = width;
	m_viewport[3] = height;

	Matrix projection, view;

	float halfWidth = static_cast<float>(width) * 0.5f;
	float halfHeight = static_cast<float>(height) * 0.5f;

	Matrix::frustum(projection, -halfWidth, halfWidth, -halfHeight, halfHeight, 10, 110);
	Matrix::lookAt(view, {0, 0, -10.0f}, {0, 0, 1}, {0, 1, 0});
	m_viewProjection = projection * view;
}

void Engine::touch(float x, float y)
{

}