#include "updater.h"

namespace Electric {

    void Updater::advance(float dt)
    {

    }

    void Updater::updateInstances(Engine::Mesh<Vertex, ParticleInstance>& mesh)
    {
        std::vector<ParticleInstance> particles;
        for(auto& it : m_particles)
        {
            particles.push_back({
                                it.pos.x(), it.pos.y(), it.pos.z(),
                                it.size,
                                it.color.r(), it.color.g(), it.color.b(), it.color.a()
                                });
        }

        mesh.updateInstances(static_cast<UINT>(particles.size()), &particles[0]);
    }
}