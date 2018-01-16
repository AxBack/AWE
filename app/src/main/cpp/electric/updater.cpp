#include "updater.h"

#include <algorithm>

namespace Electric {

float OSMOSIS_PER_SECOND = 0.05f;
float CHARGE_FLOW_FACTOR = 0.025f;
float DISCHARGE_FACTOR = 1.0f;
float LOSS_FACTOR = 0.25f;

	bool Updater::init()
	{
		Math::Matrix offset;
		Math::Matrix::identity(offset);
		setupCluster(offset, 400, 100);
		Math::Matrix::translate(offset, -25, -25, -25);
		setupCluster(offset, 300, 30);

		Math::Matrix::translate(offset, 50, 50, 50);
		setupCluster(offset, 300, 30);

		connectNodes(5);

		return Engine::Updater::init();
	}

	void Updater::setupCluster(Math::Matrix offset, int nrNodes, float nodeOffsetFromCluster)
	{
		cluster_ptr pCluster(new Cluster);
		pCluster->transform = offset;

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

			p = offset.transform(p);

			float c = static_cast<float>(chargeDist(m_generator));

			pCluster->nodes.push_back({static_cast<UINT>(m_nodeInstances.size()), p, c, 0.0f});
			m_nodeInstances.push_back({p.x(), p.y(), p.z(), c});
		}

		m_clusters.push_back(pCluster);
	}

	void Updater::connectNodes(int nrConnectionsPerNode)
	{
		for(UINT x=0; x<m_clusters.size(); ++x)
		{
			for(UINT i=0; i<m_clusters[x]->nodes.size(); ++i)
			{
				for(UINT j=i+1; j<m_clusters[x]->nodes.size(); ++j)
				{
					Math::Vector3 distance = m_clusters[x]->nodes[i].pos - m_clusters[x]->nodes[j].pos;
					float l2 = distance.lengthSq();

					Node* p1 = & m_clusters[x]->nodes[i];
					Node* p2 = & m_clusters[x]->nodes[j];

					p1->connections.push_back({p2, l2});
					p2->connections.push_back({p1, l2});
				}

				for(UINT y=x+1; y<m_clusters.size(); ++y)
				{
					for(UINT j=i+1; j<m_clusters[y]->nodes.size(); ++j)
					{
						Math::Vector3 distance = m_clusters[x]->nodes[i].pos - m_clusters[y]->nodes[j].pos;
						float l2 = distance.lengthSq();

						Node* p1 = & m_clusters[x]->nodes[i];
						Node* p2 = & m_clusters[y]->nodes[j];

						p1->connections.push_back({p2, l2});
						p2->connections.push_back({p1, l2});
					}
				}

				std::sort(m_clusters[x]->nodes[i].connections.begin(), m_clusters[x]->nodes[i].connections.end(),
						  [](const Connection& lhs, const Connection& rhs) {
							  return lhs.lengthSq < rhs.lengthSq;
						  });
				m_clusters[x]->nodes[i].connections.resize(nrConnectionsPerNode);
			}
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

	void Updater::updateCluster(cluster_ptr pCluster, float dt)
	{
		float flow = CHARGE_FLOW_FACTOR * dt;
		for(auto& it : pCluster->nodes)
		{
			it.charge += dt * OSMOSIS_PER_SECOND;

			if(it.restitution > 0.0f)
				it.restitution -= dt;

			float f = std::min(flow, it.charge);
			it.charge -= f;

			if(it.connections.size() > 0)
			{
				std::vector<Node*> lowerNodes;
				for(auto& c : it.connections)
				{
					if(it.charge < c.pNode->charge)
						lowerNodes.push_back(c.pNode);
				}

				if(lowerNodes.size() > 0)
				{
					f /= static_cast<float>(lowerNodes.size());
					for(auto& c : lowerNodes)
					{
						c->charge += f;
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
						if(it.connections[i].pNode->restitution <= 0.0f
						   && currMin > it.connections[i].pNode->charge)
						{
							index = i;
							currMin = it.connections[i].pNode->charge;
						}
					}

					if(index >= 0)
					{
						it.connections[index].pNode->charge += f * LOSS_FACTOR;
						it.charge -= f;

						it.restitution = 1.0f;
						std::lock_guard<std::mutex> _(m_dischargeMutex);
						m_charges.push_back({0.1f, it.pos, it.connections[index].pNode->pos});
					}
				}
			}
		}
	}

	void Updater::updateNodeInstances()
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		for(auto& cluster : m_clusters)
		{
			for(auto& node : cluster->nodes)
				m_nodeInstances[node.instanceId].charge = node.charge;
		}
	}

    void Updater::advance(float dt)
    {
		updateCharges(dt);
		for(auto& it : m_clusters)
			updateCluster(it, dt);
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

        mesh.updateInstances(static_cast<UINT>(particles.size()), particles.size() == 0 ? nullptr : &particles[0]);
    }

	void Updater::updateInstances(Engine::Mesh<PositionVertex, NodeInstance>& mesh)
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		mesh.updateInstances(static_cast<UINT>(m_nodeInstances.size()), m_nodeInstances.size() == 0 ? nullptr : &m_nodeInstances[0]);
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

		mesh.updateInstances(static_cast<UINT>(charges.size()), charges.size() > 0 ? &charges[0] : nullptr);
	}
}