#include "updater.h"

#include <algorithm>
#include <float.h>

namespace Electric {

float OSMOSIS_PER_SECOND = 0.05f;
float CHARGE_FLOW_FACTOR = 0.025f;
float DISCHARGE_FACTOR = 1.0f;
float LOSS_FACTOR = 0.25f;
float DISCHARGE_RADIUS_SQ = 10.0f * 10.0f;

	bool Updater::init()
	{
		setupCluster(400, 100, {0,0,0}, {0,0,0});
		setupCluster(300, 30, {-25, -25, -25}, {0,0,0});
		setupCluster(300, 30, {25, 25, 25}, {0,0,0});

		connectNodes(5);

		return Engine::Updater::init();
	}

	void Updater::setupCluster(int nrNodes, float nodeOffsetFromCluster, const Math::Vector3& pos,
								const Math::Vector3& rotation)
	{
		cluster_ptr pCluster(new Cluster);
		pCluster->dirty = true;
		pCluster->position = pos;
		pCluster->rotation = rotation;

		std::uniform_real_distribution<> posDist(-1, 1);
		std::uniform_real_distribution<> distanceDist(-nodeOffsetFromCluster, nodeOffsetFromCluster);
		std::uniform_real_distribution<> chargeDist(0.0f, 1.0f);
		std::uniform_real_distribution<> colorDist(0.0f, 1.0f);

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

			Math::Vector3 color = {
					static_cast<float>(colorDist(m_generator)),
					static_cast<float>(colorDist(m_generator)),
					static_cast<float>(colorDist(m_generator))
			};
			color.normalize();
			pCluster->nodes.push_back({static_cast<UINT>(m_nodeInstances.size()), p, p, c, 0.0f, true});
			m_nodeInstances.push_back({p.x(), p.y(), p.z(), c, color.x(), color.y(), color.z()});
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
					Math::Vector3 distance = m_clusters[x]->nodes[i].offset - m_clusters[x]->nodes[j].offset;
					float l2 = distance.lengthSq();

					Node* p1 = & m_clusters[x]->nodes[i];
					Node* p2 = & m_clusters[x]->nodes[j];

					p1->connections.push_back({p2, l2});
					p2->connections.push_back({p1, l2});
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
		bool dirty = pCluster->dirty;
		if(dirty)
		{
			Math::Matrix::setRotate(pCluster->transform,
									pCluster->rotation.x(),
									pCluster->rotation.y(),
									pCluster->rotation.z());

			Math::Matrix::translate(pCluster->transform,
									pCluster->position.x(),
									pCluster->position.y(),
									pCluster->position.z());
		}

		for(auto& it : pCluster->nodes)
		{
			if(dirty)
			{
				it.dirty = dirty;
				it.position = pCluster->transform.transform(it.offset);
			}
			updateNode(&it, dt);
		}
	}

	void Updater::updateNode(Node* pNode, float dt)
	{
		float flow = CHARGE_FLOW_FACTOR * dt;
		pNode->charge += dt * OSMOSIS_PER_SECOND;

		if(pNode->restitution > 0.0f)
			pNode->restitution -= dt;

		float f = std::min(flow, pNode->charge);
		pNode->charge -= f;

		if(pNode->connections.size() > 0)
		{
			std::vector<Node*> lowerNodes;
			for(auto& c : pNode->connections)
			{
				if(pNode->charge < c.pNode->charge)
					lowerNodes.push_back(c.pNode);
			}

			if(lowerNodes.size() > 0)
			{
				f /= static_cast<float>(lowerNodes.size());
				for(auto& c : lowerNodes)
				{
					c->charge += f;
					pNode->charge -= f;
				}
			}

			if(pNode->charge > 1.0f)
				discharge(pNode);
		}
	}

	void Updater::discharge(Node* pNode)
	{
		float radius = DISCHARGE_RADIUS_SQ * pNode->charge;
		std::vector<Node*> near;
		for(auto& cluster : m_clusters)
		{
			for(auto& node : cluster->nodes)
			{
				if(&node != pNode)
				{
					if((node.position - pNode->position).lengthSq() <=  radius)
						near.push_back(&node);
				}
			}
		}

		if(near.size() == 0)
			return;

		float f = pNode->charge * DISCHARGE_FACTOR;
		Node* p = nullptr;
		float currMin = FLT_MAX;
		for(int i=0; i<near.size(); ++i)
		{
			if(near[i]->restitution <= 0.0f
			   && currMin > near[i]->charge)
			{
				p = near[i];
				currMin = near[i]->charge;
			}
		}

		if(p)
		{
			p->charge += f * LOSS_FACTOR;
			pNode->charge -= f;

			pNode->restitution = 1.0f;
			std::lock_guard<std::mutex> _(m_dischargeMutex);
			m_charges.push_back({0.1f, pNode->position, p->position});
		}
	}

	void Updater::updateNodeInstances()
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		for(auto& cluster : m_clusters)
		{
			for(auto& node : cluster->nodes)
			{
				NodeInstance* pInstance = &m_nodeInstances[node.instanceId];
				pInstance->charge = node.charge;
				if(node.dirty)
				{
					node.dirty = false;
					pInstance->x = node.position.x();
					pInstance->y = node.position.y();
					pInstance->z = node.position.z();
				}
			}
		}
	}

    void Updater::advance(float dt)
    {
		updateCharges(dt);
		for(auto& it : m_clusters)
			updateCluster(it, dt);
		updateNodeInstances();
    }

    void Updater::updateInstances(Engine::InstancedMesh<PositionVertex, ParticleInstance>& mesh)
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

	void Updater::updateInstances(Engine::InstancedMesh<PositionVertex, NodeInstance>& mesh)
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		mesh.updateInstances(static_cast<UINT>(m_nodeInstances.size()), m_nodeInstances.size() == 0 ? nullptr : &m_nodeInstances[0]);
	}

	void Updater::updateInstances(Engine::InstancedMesh<ChargeVertex, ChargeInstance>& mesh)
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