#pragma once

#include "../engine/engine.h"
#include "updater.h"

#include "../engine/mesh.h"
#include "particle_shader.h"

namespace Electric {

    typedef Engine::Mesh<Vertex, ParticleInstance> ParticleMesh;
    typedef Engine::Camera Camera;

    class ElectricEngine : public Engine::Engine
    {
    private:

        Camera 	m_camera;

        Updater     	m_updater;

        ParticleMesh    m_particlesMesh;
        ParticleShader  m_particleShader;

    public:

        ElectricEngine()
        {
        }

        virtual void clear() override;
        virtual bool init(AAssetManager* pAssetManager) override;
        virtual bool render() override;

        virtual void resume() override { m_updater.resume(); }
        virtual void pause() override { m_updater.pause(); }

        virtual void updateSize(int width, int height) override
        {
            Engine::Engine::updateSize(width, height);
            m_camera.updateProjection(static_cast<float>(width), static_cast<float>(height));
        }

        virtual void setOffset(float x, float y) override
        {
            Engine::Engine::setOffset(x, y);
            m_camera.updateView(x);
        }
    };

}