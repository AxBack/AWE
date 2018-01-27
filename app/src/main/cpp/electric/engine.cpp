#include "engine.h"

namespace Electric {

    void ElectricEngine::clear()
    {

        LOGI("ElectricEngine( Cleared: %d )", m_id);
    }

    bool ElectricEngine::init(const char* internalFilesPath, AAssetManager* pAssetManager)
    {
        LOGI("ElectricEngine( Init begin: %d )", m_id);

		if(!setupPostProcess(pAssetManager))
			return false;

        if(!setupParticles(pAssetManager))
			return false;

		if(!setupNodes(pAssetManager))
			return false;

		if(!setupDischarges(pAssetManager))
			return false;

        if(!m_updater.init(internalFilesPath))
            return false;

        LOGI("ElectricEngine( Init end: %d )", m_id);
        return true;
    }

	bool ElectricEngine::setupPostProcess(AAssetManager* pAssetManager)
	{
		TexturedVertex vertices[] = {
				{-1,-1,0,0,0},
				{ 1,-1,0,1,0},
				{ 1, 1,0,1,1},
				{-1, 1,0,0,1}
		};

		GLushort indices[] = {0,1,2,0,2,3};
		if(!m_screenMesh.init(4, vertices, 6, indices))
		{
			LOGD("init( Failed to init ScreenMesh: %d )", m_id);
			return false;
		}

		if(!m_bloomShader.init(pAssetManager, m_screenMesh))
		{
			LOGD("init( Failed to init BloomShader: %d )", m_id);
			return false;
		}

		if(!m_dofShader.init(pAssetManager, m_screenMesh))
		{
			LOGD("init( Failed to init DofShader: %d )", m_id);
			return false;
		}
		return true;
	}

	bool ElectricEngine::setupParticles(AAssetManager* pAssetManager)
	{
		PositionVertex vertices[] = {
				{-0.5f, 0.5f,  0},
				{0.5f,  0.5f,  0},
				{0.5f,  -0.5f, 0},
				{-0.5f, -0.5f, 0}
		};

		GLushort indices[] = {0, 1, 2, 0, 2, 3};
		if (!m_particlesMesh.init(4, vertices, 6, indices))
		{
			LOGD("init( Failed to init ParticleMesh: %d )", m_id);
			return false;
		}

		if (!m_particleShader.init(pAssetManager, m_particlesMesh))
		{
			LOGD("init( Failed to init ParticleShader: %d )", m_id);
			return false;
		}
		return true;
	}

	bool ElectricEngine::setupNodes(AAssetManager* pAssetManager)
	{
		PositionVertex vertices[] = {
				{-0.5f, 0.5f,  0},
				{0.5f,  0.5f,  0},
				{0.5f,  -0.5f, 0},
				{-0.5f, -0.5f, 0}
		};

		GLushort indices[] = {0, 1, 2, 0, 2, 3};
		if (!m_nodeMesh.init(4, vertices, 6, indices))
		{
			LOGD("init( Failed to init NodeMesh: %d )", m_id);
			return false;
		}

		if (!m_nodeShader.init(pAssetManager, m_nodeMesh))
		{
			LOGD("init( Failed to init NodeShader: %d )", m_id);
			return false;
		}

		return true;
	}

	bool ElectricEngine::setupDischarges(AAssetManager* pAssetManager)
	{
		DischargeVertex vertices[] = {
				{0.0f, 0.0f,0.0f, 0.0f, 0.0f},
				{0.0f, 0.0f,0.0f, 0.0f, 1.0f},
				{0.0f, 0.2f,0.0f, 1.0f, 0.2f},
				{0.0f, 0.2f,0.0f, 1.0f, 0.4f},
				{0.0f, 0.2f,0.0f, 1.0f, 0.6f},
				{0.0f, 0.2f,0.0f, 1.0f, 0.8f},
				{0.0f,-0.2f,0.0f, 1.0f, 0.2f},
				{0.0f,-0.2f,0.0f, 1.0f, 0.4f},
				{0.0f,-0.2f,0.0f, 1.0f, 0.6f},
				{0.0f,-0.2f,0.0f, 1.0f, 0.8f}
		};

		GLushort indices[] = {
				0,2,6, 5,1,9, // ends
				2,3,7, 2,7,6,
				3,4,8, 3,8,7,
				4,5,9, 4,9,8
		};

		if(!m_dischargeMesh.init(10, vertices, 3*8, indices))
		{
			LOGD("init( Failed to init ChargeMesh: %d )", m_id);
			return false;
		}

		std::mt19937 generator(840331);
		std::uniform_int_distribution<char> dist(0, 255);

		const int nrBytes = 8*8*3;
		char pixels[nrBytes];
		for(int i=0; i<nrBytes; ++i)
			pixels[i] = dist(generator);

		if(!m_dischargeTexture.init(GL_RGB, 8, 8, pixels))
		{
			LOGD("init( Failed to init DischargeTexture: %d )", m_id);
			return false;
		}

		if(!m_dischargeShader.init(pAssetManager, m_dischargeMesh))
		{
			LOGD("init( Failed to init DischargeShader: %d )", m_id);
			return false;
		}
		return true;
	}

	void ElectricEngine::restart()
	{
		m_updater.restart();
	}

    bool ElectricEngine::render()
    {
		if(m_sizeDirty)
		{
			std::lock_guard<std::mutex> _(m_sizeMutex);
			m_sizeDirty = false;
			GLint formats[] = {GL_RGB, GL_RGB};
			m_renderTarget.init(m_viewport[2], m_viewport[3], 2, formats, Framebuffer::READ_WRITE);
			m_bloomShader.updateSize(m_viewport[2], m_viewport[3]);
			m_dofShader.updateSize(m_viewport[2], m_viewport[3]);
            m_camera.updateProjection(m_viewport[2], m_viewport[3]);
		}

		m_updater.updateInstances(m_dischargeMesh);
		m_updater.updateInstances(m_nodeMesh);
		m_updater.updateInstances(m_particlesMesh);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);

        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

		glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			Math::Matrix r;
			Math::Matrix::setRotate(r, 0,0, m_rollPath.traverse(m_rotation));
			Math::Vector3 up = Math::Matrix::transform(r, {0,1,0});

			//TODO:create update on demand
			Math::Matrix y;
			Math::Matrix::setRotate(y, 0,m_yawPath.traverse(m_offset), 0);
			y = r * y;
			Math::Vector3 pos = Math::Matrix::transform(y, m_positionPath.traverse(m_pinch));

			m_camera.updateView(pos, {1,0,0}, up);
		}
        m_camera.update();

		m_renderTarget.set();
		m_renderTarget.clear();

		if(m_nodeMesh.hasInstances())
			m_nodeShader.render(m_camera, m_nodeMesh);

		glBlendFunc(GL_ONE, GL_ONE);

		if(m_dischargeMesh.hasInstances())
			m_dischargeShader.render(m_camera, m_dischargeMesh, m_dischargeTexture);

		if(m_particlesMesh.hasInstances())
			m_particleShader.render(m_camera, m_particlesMesh);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

		m_dofShader.render(m_screenMesh, m_renderTarget, 0);
		m_bloomShader.render(m_screenMesh, m_renderTarget, 1);

        return true;
    }
}