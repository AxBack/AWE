#include "engine.h"

namespace Connections {

    void ConnectionsEngine::clear() {
        m_pointShader.release();
        m_pointMesh.clean();
        m_updater.pause();
        LOGI("ConnectionsEngine( Cleared: %d )", m_id);
    }

    bool ConnectionsEngine::init(const char*, AAssetManager *pAssetManager) {
        clear();

        LOGI("ConnectionsEngine( Init begin: %d )", m_id);

        {
            Vertex vertices[] = {
                    {-0.5f, 0.5f,  0, 0},
                    {0.5f,  0.5f,  0, 0},
                    {0.5f,  -0.5f, 0, 0},
                    {-0.5f, -0.5f, 0, 0}
            };

            GLushort indices[] = {0, 1, 2, 0, 2, 3};
            if (!m_pointMesh.init(4, vertices, 6, indices)) {
                LOGD("init( Failed to init PointMesh: %d )", m_id);
                return false;
            }

            if (!m_pointShader.init(pAssetManager, m_pointMesh)) {
                LOGD("init( Failed to init PointShader: %d )", m_id);
                return false;
            }
        }

        {
            Vertex vertices[] = {
                    {-1.0f, 0.0f,  0, 0},
                    {1.0f,  0.0f,  0, 0},
                    {-1.0f, 0.25f, 0, 0.7f},
                    {1.0f,  0.25f, 0, 0.7f},
                    {-1.0f, 0.5f,  0, 1},
                    {1.0f,  0.5f,  0, 1},
                    {-1.0f, 0.75f, 0, 0.7f},
                    {1.0f,  0.75f, 0, 0.7f},
                    {-1.0f, 1.0f,  0, 0},
                    {1.0f,  1.0f,  0, 0}
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
            if (!m_connectionMesh.init(10, vertices, 24, indices)) {
                LOGD("init( Failed to init ConnectionMesh: %d )", m_id);
                return false;
            }

            if (!m_connectionShader.init(pAssetManager, m_connectionMesh)) {
                LOGD("init( Failed to init ConnectionShader: %d )", m_id);
                return false;
            }
        }

        if (!m_updater.init()) {
            LOGD("init( Failed to init updater: %d )", m_id);
            return false;
        }

        LOGI("ConnectionsEngine( Init end: %d )", m_id);

        return true;
    }

    bool ConnectionsEngine::render()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!m_updater.updateInstances(m_pointMesh, m_connectionMesh))
            return false;

        Math::Matrix vp;
        {
            std::lock_guard<std::mutex> _(m_matrixMutex);
            Math::Matrix rotation;
            Math::Matrix::setRotate(rotation, 0, 360.0f * (m_offset - 0.5f), 0);
            Math::Vector3 pos = Math::Matrix::transform(rotation, {0, 0, -100.0f});
            m_camera.updateView(pos, {0,0,0}, {0,1,0});
            m_camera.update();
        }

        m_connectionShader.render(m_camera, m_connectionMesh);
        m_pointShader.render(m_camera, m_pointMesh);

        return true;
    }

    void ConnectionsEngine::updateSize(int width, int height) {
        Engine::updateSize(width, height);
		std::lock_guard<std::mutex> _(m_matrixMutex);
		m_camera.updateProjection(width, height);
    }

    void ConnectionsEngine::updateView(float offset)
    {
        std::lock_guard<std::mutex> _(m_matrixMutex);
        m_offset = offset;
    }

    void ConnectionsEngine::touch(float x, float y) {
        /*Math::Vector3 start = {x - m_viewport[2] * 0.5f, y - m_viewport[3] * 0.5f, 0.0f};
        Math::Vector3 end = m_inverseViewProjection.project(start);
        m_updater.rayTest(start, end);*/
    }
}