#pragma once

#include "../engine/engine.h"
#include "updater.h"

#include "../engine/mesh.h"
#include "particle_shader.h"
#include "node_shader.h"
#include "charge_shader.h"
#include "../engine/framebuffer.h"
#include "../engine/path.h"
#include "../engine/vector3.h"
#include "bloom_shader.h"
#include "dof_shader.h"

namespace Electric {

	typedef Engine::Mesh<TexturedVertex> ScreenMesh;
    typedef Engine::InstancedMesh<PositionVertex, ParticleInstance> ParticleMesh;
    typedef Engine::InstancedMesh<PositionVertex, NodeInstance> NodeMesh;
	typedef Engine::InstancedMesh<ChargeVertex, ChargeInstance> ChargeMesh;
    typedef Engine::Camera Camera;
	typedef Engine::Framebuffer Framebuffer;
    typedef Engine::Path<Math::Vector3> vec3_path;
	typedef Engine::Path<float> float_path;
	typedef std::atomic<float> atomic_float;

    class ElectricEngine : public Engine::Engine
    {
    private:

        //temp
        float           m_scaleTime;
		float_path		m_scalePath;

		atomic_float 	m_pinch;
        vec3_path       m_positionPath;

		atomic_float 	m_offset;
        float_path	    m_yawPath;

		atomic_float	m_rotation;
		float_path		m_rollPath;

        Camera 			m_camera;

        Updater     	m_updater;

		ScreenMesh		m_screenMesh;
		BloomShader		m_bloomShader;
		DofShader		m_dofShader;

        ParticleMesh    m_particlesMesh;
        ParticleShader  m_particleShader;

        NodeMesh        m_nodeMesh;
        NodeShader      m_nodeShader;

		ChargeMesh		m_chargeMesh;
		DischargeShader	m_chargeShader;

		std::atomic_bool m_sizeDirty;
		std::mutex 		 m_sizeMutex;
		Framebuffer		 m_renderTarget;

    public:

        ElectricEngine()
		: m_sizeDirty(false)
        , m_scaleTime(0.0f)
        , m_offset(0.5f)
		, m_pinch(0.5f)
		, m_rotation(0.0f)
        {
            {
                Math::Vector3 points[] = {
                        {0, 0, -100},
						{0, 0, -1000.0f}
                };
                m_positionPath.add(5.0f, 2, points);
            }

			{
				float scales[] = {0.0f, 0.75f, 0.90f, 0.95f, 1.0f };
				m_scalePath.add(5, 5, scales);
			};

            {
                float points[] = { -180.0f, 180.0f};
                m_yawPath.add(1.0f, 2, points);
            }

			{
				float points[] = { 0, -360};
				m_yawPath.add(360, 2, points);
			}
        }

        virtual void clear() override;
        virtual bool init(AAssetManager* pAssetManager) override;
        virtual bool render() override;

        virtual void resume() override { m_updater.resume(); }
        virtual void pause() override { m_updater.pause(); }

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

		virtual void rotate(float angle) override
		{
			Engine::Engine::rotate(angle);
			m_rotation = angle;
		}
    };

}