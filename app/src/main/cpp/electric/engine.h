#pragma once

#include "../engine/engine.h"
#include "updater.h"

#include "particle_shader.h"
#include "node_shader.h"
#include "discharge_shader.h"
#include "../engine/framebuffer.h"
#include "bloom_shader.h"
#include "dof_shader.h"
#include "../engine/texture.h"
#include "../engine/sensor.h"
#include "node_overlay_shader.h"

#include <chrono>

namespace Electric {

	class Updater;

	typedef Engine::Mesh<TexturedVertex> ScreenMesh;
    typedef Engine::InstancedMesh<PositionVertex, ParticleInstance> ParticleMesh;
    typedef Engine::InstancedMesh<PositionVertex, NodeInstance> NodeMesh;
	typedef Engine::InstancedMesh<DischargeVertex, DischargeInstance> ChargeMesh;
    typedef Engine::Camera Camera;
	typedef Engine::Framebuffer Framebuffer;
	typedef Engine::Texture Texture;
    typedef Engine::Path<Math::Vector3> vec3_path;
	typedef Engine::Path<float> float_path;
	typedef Engine::Sensor Sensor;
	typedef std::atomic<float> atomic_float;

    class ElectricEngine : public Engine::Engine
    {
    private:
		Sensor 			m_sensor;

		atomic_float 	m_pinch;
        vec3_path       m_positionPath;

		atomic_float 	m_offset;
        float_path	    m_yawPath;

		float			m_rotation;
		float_path		m_pitchPath;

        Camera 			m_camera;

		Updater 		m_updater;

		ScreenMesh		m_screenMesh;
		BloomShader		m_bloomShader;
		DofShader		m_dofShader;

        ParticleMesh    m_particlesMesh;
        ParticleShader  m_particleShader;

        NodeMesh        m_nodeMesh;
        NodeShader      m_nodeShader;
		NodeOverlayShader m_nodeOverlayShader;

		ChargeMesh		m_dischargeMesh;
		Texture			m_dischargeTexture;
		DischargeShader	m_dischargeShader;

		std::atomic_bool m_sizeDirty;
		std::mutex 		 m_sizeMutex;
		Framebuffer		 m_mainTarget;
		Framebuffer		 m_bloomTarget;

		bool setupPostProcess(AAssetManager* pAssetManager);
		bool setupParticles(AAssetManager* pAssetManager);
		bool setupNodes(AAssetManager* pAssetManager);
		bool setupDischarges(AAssetManager* pAssetManager);

    public:

        ElectricEngine()
		: m_sizeDirty(false)
        , m_offset(0.5f)
		, m_pinch(0.75f)
        {
            {
                Math::Vector3 points[] = {
                        {0, 0, -50},
						{0, 0, -300.0f}
                };
                m_positionPath.add(5.0f, 2, points);
            }

            {
                float points[] = { -180.0f, 180.0f};
                m_yawPath.add(1.0f, 2, points);
            }

			{
				float points[] = { 0, 45, -45, 0};
				m_pitchPath.add(100, 2, points);
			}
        }

        virtual void clear() override;
        virtual bool init(const char* internalFilePath, AAssetManager* pAssetManager) override;
		virtual void reset(const char* internalFilePath) override;
        virtual bool render() override;

        virtual void resume() override
		{
			m_sensor.resume();
			m_updater.resume();
		}
        virtual void pause() override
		{
			m_sensor.pause();
			m_updater.pause();
		}

        virtual void updateSize(int width, int height) override
        {
			if(width != m_viewport[2] || height != m_viewport[3])
			{
				m_sizeDirty = true;
				std::lock_guard<std::mutex> _(m_sizeMutex);
				Engine::Engine::updateSize(width, height);
				m_camera.updateProjection(static_cast<float>(width), static_cast<float>(height));
			}
		}

        virtual void setOffset(float x, float y) override
        {
            m_offset = x;
        }

		virtual void pinch(float diff) override
		{
			Engine::Engine::pinch(diff);
			float c = m_pinch;
			m_pinch = std::max(0.0f, std::min(m_positionPath.getLength(), c + diff));
		}

		void setState(const Cluster::State& state)
		{
			m_updater.setState(0, state);
		}
    };

}