#include "engine.h"

#include <sstream>

namespace Electric {

    void ElectricEngine::clear()
    {

        LOGI("ElectricEngine( Cleared: %d )", m_id);
    }

    bool ElectricEngine::init(const char* internalFilePath, AAssetManager* pAssetManager)
    {
        LOGI("ElectricEngine( Init begin: %d )", m_id);

		if(!m_sensor.init(m_id))
			return false;

		if(!setupPostProcess(pAssetManager))
			return false;

        if(!setupParticles(pAssetManager))
			return false;

		if(!setupNodes(pAssetManager))
			return false;

		if(!setupDischarges(pAssetManager))
			return false;

        if(!m_updater.init(internalFilePath))
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

		if (!m_nodeOverlayShader.init(pAssetManager, m_nodeMesh))
		{
			LOGD("init( Failed to init NodeOverlayShader: %d )", m_id);
			return false;
		}

		return true;
	}

	bool ElectricEngine::setupDischarges(AAssetManager* pAssetManager)
	{
		float size = 0.5f;
		DischargeVertex vertices[] = {
				{-size, size,0.0f, 0,0}, // 0
				{ size, size,0.0f, 1,0}, // 1
				{ size,-size,0.0f, 1,1}, // 2
				{-size,-size,0.0f, 0,1}  // 3
		};

		GLushort indices[] = {0,1,2,0,2,3};

		if(!m_dischargeMesh.init(10, vertices, 3*8, indices))
		{
			LOGD("init( Failed to init ChargeMesh: %d )", m_id);
			return false;
		}

		std::mt19937 generator;
		std::uniform_int_distribution<char> dist(0, static_cast<char>(255));

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

	void ElectricEngine::reset(const char* internalFilePath)
	{
		m_updater.reset(internalFilePath);
	}

	void ElectricEngine::updatePath(::Engine::IO::BinaryReader* pReader)
	{
		m_updater.updatePath(0, 0, pReader);
	}

    bool ElectricEngine::render()
    {
		if(m_sizeDirty)
		{
			std::lock_guard<std::mutex> _(m_sizeMutex);
			m_sizeDirty = false;
			GLint formats[] = {GL_RGBA};
			m_mainTarget.init(m_viewport[2], m_viewport[3], 1, formats, Framebuffer::READ_WRITE);
			m_bloomTarget.init(m_viewport[2], m_viewport[3], 1, formats, Framebuffer::NONE);
			//m_bloomTarget.attachDepthBuffer(m_mainTarget.getDepthBuffer(), Framebuffer::WRITE);
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
			//TODO:create update on demand
			m_sensor.update();
			Math::Vector3 r = m_sensor.getRotation();

			Math::Quaternion pitch = Math::Quaternion::fromAxisAngle({1,0,0}, r.x());
			Math::Quaternion yaw = Math::Quaternion::fromAxisAngle({0,1,0}, -r.y());
			Math::Quaternion roll = Math::Quaternion::fromAxisAngle({0,0,1}, r.z());

			Math::Matrix tilt;
			Math::Matrix::setRotate(tilt, pitch * yaw * roll);

			Math::Vector3 at = Math::Matrix::transform(tilt, {0,0,1}, 0.0f);
			Math::Vector3 up = {0,1,0};

			Math::Quaternion offset = Math::Quaternion::fromAxisAngle(0,1,0, -m_yawPath.traverse(m_offset));

			Math::Matrix rot;
			Math::Matrix::setRotate(rot, offset);

			Math::Vector3 pos = Math::Matrix::transform(rot, m_positionPath.traverse(m_pinch));
			at = Math::Matrix::transform(rot, at, 0.0f);
			up = Math::Matrix::transform(rot, up, 0.0f);

			m_camera.updateView(pos, pos + at, up);
		}
        m_camera.update();

		m_mainTarget.set();
		m_mainTarget.clear();

		if(m_nodeMesh.hasInstances())
			m_nodeShader.render(m_camera, m_nodeMesh);

		if(m_dischargeMesh.hasInstances())
			m_dischargeShader.render(0.5f, m_camera, m_dischargeMesh, m_dischargeTexture);

		m_bloomTarget.set();
		m_bloomTarget.clear();

		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);

		if(m_nodeMesh.hasInstances())
			m_nodeOverlayShader.render(m_camera, m_nodeMesh);

		if(m_dischargeMesh.hasInstances())
			m_dischargeShader.render(1.0f, m_camera, m_dischargeMesh, m_dischargeTexture);

		if(m_particlesMesh.hasInstances())
			m_particleShader.render(m_camera, m_particlesMesh);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

		m_dofShader.render(m_screenMesh, m_mainTarget, 0);
		m_bloomShader.render(m_screenMesh, m_bloomTarget, 0);

        return true;
    }
}