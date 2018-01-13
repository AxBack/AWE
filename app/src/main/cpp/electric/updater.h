#pragma once

#include "../engine/updater.h"
#include "../engine/vector3.h"
#include "../engine/color.h"
#include "vertex.h"

namespace Electric {

    class Updater : public Engine::Updater
    {
    private:

        struct Particle
        {
            Math::Vector3 pos;
            float size;
            Math::Color color;
        };

        typedef std::vector<Particle> particle_vec;

        particle_vec m_particles;

    protected:

        virtual void advance(float dt) override;

    public:

        Updater()
        {
            m_particles.push_back({{0,0,0},10,{1,1,1,1}});
			m_particles.push_back({{0,0,10}, 50, {1,0.5,0,0.5}});
		}

        void updateInstances(Engine::Mesh<Vertex, ParticleInstance>& mesh);

    };
}
