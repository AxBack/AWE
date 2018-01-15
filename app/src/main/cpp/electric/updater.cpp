#include "updater.h"

#include <algorithm>

namespace Electric {

float OSMOSIS_PER_SECOND = 0.05f;
float CHARGE_FLOW_FACTOR = 0.025f;
float DISCHARGE_FACTOR = 1.0f;
float LOSS_FACTOR = 0.25f;

	bool Updater::init()
	{
		setupCluster(1000, 5, 100);

		return Engine::Updater::init();
	}

	void Updater::setupCluster(int nrNodes, int nrConnectionsPerNode, float nodeOffsetFromCluster)
	{
		std::uniform_real_distribution<> posDist(-1, 1);
		std::uniform_real_distribution<> distanceDist(-nodeOffsetFromCluster, nodeOffsetFromCluster);
		std::uniform_real_distribution<> chargeDist(0.0f, 1.0f);

		for(int i = 0; i < nrNodes; ++i)
		{
			Math::Vector3 p = {
					static_cast<float>(posDist(m_generator)),
					static_cast<float>(posDist(m_generator)),
					static_cast<float>(posDist(m_generator))
			};

			p.normalize();
			p *= static_cast<float>(distanceDist(m_generator));

			float c = static_cast<float>(chargeDist(m_generator));

			m_nodes.push_back({p, c, 0.0f});
			m_nodeInstances.push_back({p.x(), p.y(), p.z(), c});
		}

		for(UINT x = 0; x < m_nodes.size(); ++x)
		{
			for(UINT y = x + 1; y < m_nodes.size(); ++y)
			{
				Math::Vector3 distance = m_nodes[x].pos - m_nodes[y].pos;
				float l2 = distance.lengthSq();
				m_nodes[x].connections.push_back({y, l2});
				m_nodes[y].connections.push_back({x, l2});
			}

			std::sort(m_nodes[x].connections.begin(), m_nodes[x].connections.end(),
					  [](const Connection& lhs, const Connection& rhs) {
						  return lhs.lengthSq < rhs.lengthSq;
					  });

			m_nodes[x].connections.resize(nrConnectionsPerNode);
		}
	}

	void Updater::updateCharges(float dt)
	{
		std::lock_guard<std::mutex> _(m_dischargeMutex);
		for(auto it = m_charges.begin(); it != m_charges.end(); )
		{
			it->time -= dt;
			if(it->time <= 0.0f)
				it = m_charges.erase(it);
			else
				++it;
		}
	}

	void Updater::updateNodes(float dt)
	{
		float flow = CHARGE_FLOW_FACTOR * dt;
		for(auto& it : m_nodes)
		{
			it.charge += dt * OSMOSIS_PER_SECOND;

			if(it.restitution > 0.0f)
				it.restitution -= dt;

			float f = std::min(flow, it.charge);
			it.charge -= f;

			if(it.connections.size() > 0)
			{
				std::vector<UINT> lowerNodes;
				for(auto& c : it.connections)
				{
					if(it.charge < m_nodes[c.index].charge)
						lowerNodes.push_back(c.index);
				}

				if(lowerNodes.size() > 0)
				{
					f /= static_cast<float>(lowerNodes.size());
					for(auto& c : lowerNodes)
					{
						m_nodes[it.connections[c].index].charge += f;
						it.charge -= f;
					}
				}

				if(it.charge > 1.0f)
				{
					f = it.charge * DISCHARGE_FACTOR;
					int index = -1;
					float currMin = FLT_MAX;
					for(int i=0; i<it.connections.size(); ++i)
					{
						if(m_nodes[it.connections[i].index].restitution <= 0.0f
						   && currMin > m_nodes[it.connections[i].index].charge)
						{
							index = i;
							currMin = m_nodes[it.connections[i].index].charge;
						}
					}

					if(index >= 0)
					{
						Node* p = &m_nodes[it.connections[index].index];

						p->charge += f * LOSS_FACTOR;
						it.charge -= f;

						it.restitution = 1.0f;
						std::lock_guard<std::mutex> _(m_dischargeMutex);
						m_charges.push_back({0.1f, it.pos, p->pos});
					}
				}
			}
		}
	}

	void Updater::updateNodeInstances()
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		for(int i=0; i<m_nodes.size(); ++i)
			m_nodeInstances[i].charge = m_nodes[i].charge;
	}

    void Updater::advance(float dt)
    {
		updateCharges(dt);
		updateNodes(dt);
		updateNodeInstances();
    }

    void Updater::updateInstances(Engine::Mesh<PositionVertex, ParticleInstance>& mesh)
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

        mesh.updateInstances(particles.size(), particles.size() == 0 ? nullptr : &particles[0]);
    }

	void Updater::updateInstances(Engine::Mesh<PositionVertex, NodeInstance>& mesh)
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		mesh.updateInstances(m_nodeInstances.size(), m_nodeInstances.size() == 0 ? nullptr : &m_nodeInstances[0]);
	}

	void Updater::updateInstances(Engine::Mesh<ChargeVertex, ChargeInstance>& mesh)
	{
		std::lock_guard<std::mutex> _(m_dischargeMutex);
		std::vector<ChargeInstance> charges;
		for(auto& it : m_charges)
		{
			charges.push_back({
							  it.start.x(), it.start.y(), it.start.z(),
							  it.end.x(), it.end.y(), it.end.z()
							  });
		}

		mesh.updateInstances(charges.size(), charges.size() > 0 ? &charges[0] : nullptr);
	}
}