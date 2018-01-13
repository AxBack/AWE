#include "engine.h"

namespace Electric {

    void ElectricEngine::clear()
    {

        LOGI("ElectricEngine( Cleared: %d )", m_id);
    }

    bool ElectricEngine::init(AAssetManager* pAssetManager)
    {
        LOGI("ElectricEngine( Init begin: %d )", m_id);

        {
            Vertex vertices[] = {
                    {-0.5f, 0.5f,  0},
                    {0.5f,  0.5f,  0},
                    {0.5f,  -0.5f, 0},
                    {-0.5f, -0.5f, 0}
            };

            GLushort indices[] = {0, 1, 2, 0, 2, 3};
            if (!m_particlesMesh.init(4, vertices, 6, indices)) {
                LOGD("init( Failed to init ParticleMesh: %d )", m_id);
                return false;
            }

            if (!m_particleShader.init(pAssetManager, m_particlesMesh)) {
                LOGD("init( Failed to init ParticleShader: %d )", m_id);
                return false;
            }
        }

        if(!m_updater.init())
            return false;

        LOGI("ElectricEngine( Init end: %d )", m_id);
        return true;
    }

    bool ElectricEngine::render()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);

        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Matrix vp;
        {
            std::lock_guard<std::mutex> _(m_matrixMutex);
            if(m_vpDirty)
                m_vp = m_projection * m_view;
            vp = m_vp;
        }

		m_updater.updateInstances(m_particlesMesh);

        m_particleShader.bind(vp);
        m_particlesMesh.render();

        return true;
    }

}