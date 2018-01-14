#include "updater.h"

namespace Electric {

float CHARGE_FLOW_FACTOR = 0.125f;
float DISCHARGE_FACTOR = 0.5f;

	bool Updater::init()
	{
		{
			int nrNodes = 500;

			std::uniform_real_distribution<> pos(-1, 1);
			std::uniform_real_distribution<> distance(0, 100.0);
			std::uniform_real_distribution<> charge(0.0f, 1.0f);
			for(int i = 0; i < nrNodes; ++i)
			{
				Math::Vector3 p = {
						static_cast<float>(pos(m_generator)),
						static_cast<float>(pos(m_generator)),
						static_cast<float>(pos(m_generator))
				};

				p.normalize();
				p *= static_cast<float>(distance(m_generator));

				float c = static_cast<float>(charge(m_generator));

				m_nodes.push_back({p, c});
				m_nodeInstances.push_back({p.x(), p.y(), p.z(), c});
			}
		}

		for(UINT x = 0; x < m_nodes.size(); ++x)
		{
			float minDist = 10.0f;
			do
			{
				float connectionMinSq = minDist * minDist;
				for(UINT y = x + 1; y < m_nodes.size(); ++y)
				{
					Math::Vector3 distance = m_nodes[x].pos - m_nodes[y].pos;
					if(distance.lengthSq() < connectionMinSq)
					{
						m_nodes[x].connections.push_back(y);
						m_nodes[y].connections.push_back(x);
					}
				}

				minDist += 5.0f;

			}while(m_nodes[x].connections.size() == 0);
		}

		return Engine::Updater::init();
	}

    void Updater::advance(float dt)
    {
		float flow = CHARGE_FLOW_FACTOR * dt;
		for(auto& it : m_nodes)
		{
			float f = std::min(flow, it.charge);
			it.charge -= f;

			if(it.connections.size() > 0)
			{
				f /= static_cast<float>(it.connections.size());
				for(auto& c : it.connections)
					m_nodes[c].charge += f;

				if(it.charge > 1.0f)
				{
					f = it.charge * DISCHARGE_FACTOR;
					int index = 0;
					float currMin = FLT_MAX;
					for(int i=0; i<it.connections.size(); ++i)
					{
						if(currMin > m_nodes[it.connections[i]].charge)
						{
							index = i;
							currMin = m_nodes[it.connections[i]].charge;
						}
					}

					m_nodes[it.connections[index]].charge += f;
					it.charge -= f;
				}
			}
		}

		{
			std::lock_guard<std::mutex> _(m_nodeMutex);
			for(int i=0; i<m_nodes.size(); ++i)
				m_nodeInstances[i].charge = m_nodes[i].charge;
		}
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

        mesh.updateInstances(particles.size(), &particles[0]);
    }

	void Updater::updateInstances(Engine::Mesh<Vertex, NodeInstance>& mesh)
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		mesh.updateInstances(m_nodeInstances.size(), &m_nodeInstances[0]);
	}
}