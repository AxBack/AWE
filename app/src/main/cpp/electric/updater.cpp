#include "updater.h"

#include <random>

namespace Electric {

	bool Updater::init()
	{
		{
			int nrNodes = 500;

			std::mt19937 gen(840331);
			std::uniform_real_distribution<> pos(-1, 1);
			std::uniform_real_distribution<> distance(0, 100.0);
			std::uniform_real_distribution<> charge(0.0f, 2.0f);
			for(int i = 0; i < nrNodes; ++i)
			{
				Math::Vector3 p = {
						static_cast<float>(pos(gen)),
						static_cast<float>(pos(gen)),
						static_cast<float>(pos(gen))
				};

				p.normalize();
				p *= static_cast<float>(distance(gen));

				float c = static_cast<float>(charge(gen));

				m_nodes.push_back({p, c});
				m_nodeInstances.push_back({p.x(), p.y(), p.z(), c});
			}
		}

		float connectionMinSq = 10.0f * 10.0f;
		for(UINT x=0; x<m_nodes.size(); ++x)
		{
			for(UINT y=x+1; y<m_nodes.size(); ++y)
			{
				Math::Vector3 distance = m_nodes[x].pos - m_nodes[y].pos;
				if(distance.lengthSq() < connectionMinSq)
				{
					m_nodes[x].connections.push_back(y);
					m_nodes[y].connections.push_back(x);
				}
			}
		}

		return Engine::Updater::init();
	}

    void Updater::advance(float dt)
    {

    }

    void Updater::updateInstances(Engine::Mesh<Vertex, ParticleInstance>& mesh)
    {
		std::lock_guard<std::mutex> _(m_particleMutex);
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

	void Updater::updateInstances(Engine::Mesh<Vertex, NodeInstance>& mesh)
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		for(int i=0; i<m_nodes.size(); ++i)
			m_nodeInstances[i].charge = m_nodes[i].charge;

		mesh.updateInstances(static_cast<UINT>(m_nodeInstances.size()), &m_nodeInstances[0]);
	}
}