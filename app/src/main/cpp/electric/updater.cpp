#include "updater.h"
#include "../engine/binary_reader.h"

#include <algorithm>
#include <bits/ios_base.h>
#include <fstream>

namespace Electric {

float DISCHARGE_FACTOR = 1.0f;
float LOSS_FACTOR = 0.25f;
float DISCHARGE_RADIUS_SQ = 10.0f * 10.0f;

	bool Updater::init()
	{
		Engine::BinaryReader reader((m_internalFilesPath + "/WAE.dat").c_str());
		int nrClusters = reader.read<int>();

		for(int i=0; i<nrClusters; ++i)
		{
			loadCluster(reader);
		}
		//setupCluster(300, 30, {-25, -25, -25}, {0,0,0});
		//setupCluster(300, 30, {25, 25, 25}, {0,0,0});

		return Engine::Updater::init();
	}

	void Updater::restart()
	{
		bool running = isRunning();
		if(running)
			stop();

		clear();
		init();

		if(running)
			start();
	}

	void Updater::clear()
	{
		{
			std::lock_guard<std::mutex> _(m_particleMutex);
			m_particles.clear();
		}

		{
			std::lock_guard<std::mutex> _(m_nodeMutex);
			m_nodeInstances.clear();
			m_clusters.clear();
		}

		std::lock_guard<std::mutex> _(m_dischargeMutex);
		m_discharges.clear();
	}

	void Updater::loadCluster(Engine::BinaryReader& reader)
	{
		cluster_ptr pCluster(new Cluster);
		pCluster->init(m_generator, reader, m_nodeInstances, this);
		m_clusters.push_back(pCluster);
	}

	void Updater::updateCharges(float dt)
	{
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);

		std::lock_guard<std::mutex> _(m_dischargeMutex);
		m_dischargeInstances.clear();
		for(auto it = m_discharges.begin(); it != m_discharges.end(); )
		{
			it->rand = dist(m_generator);
			it->time -= dt;
			if(it->time <= 0.0f)
				it = m_discharges.erase(it);
			else
			{
				Math::Vector3 s = it->pStart->getPosition();
				Math::Vector3 e = it->pEnd->getPosition();
				m_dischargeInstances.push_back({
						s.x(), s.y(), s.z(),
						e.x(), e.y(), e.z(),
						it->rand
										  });
				++it;
			}
		}
	}

	void Updater::updateCluster(cluster_ptr pCluster, float dt)
	{
		pCluster->update(dt);
	}

	void Updater::onDischarge(Node* pNode)
	{
		float radius = DISCHARGE_RADIUS_SQ * pNode->getCharge();
		std::vector<SearchResult> near;
		for(auto& cluster : m_clusters)
		{
			cluster->search(pNode->getPosition(), radius, near);
		}

		if(near.size() == 0)
			return;

		SearchResult* p = nullptr;
		float currMin = FLT_MAX;
		for(int i=0; i<near.size(); ++i)
		{
			float v = (near[i].lengthSq / radius) + (pNode->getCharge() - near[i].pNode->getCharge());
			if(currMin > v)
			{
				p = &near[i];
				currMin = v;
			}
		}

		if(p)
		{
			float f = pNode->getCharge() * DISCHARGE_FACTOR;
			p->pNode->modifyCharge( f * LOSS_FACTOR );
			pNode->modifyCharge(-f);

			std::uniform_real_distribution<float> dist(0.0f, 1.0f);

			std::lock_guard<std::mutex> _(m_dischargeMutex);
			m_discharges.push_back({m_dischargeTime, pNode, p->pNode, dist(m_generator)});
		}
	}

	void Updater::updateNodeInstances()
	{
		std::lock_guard<std::mutex> _(m_nodeMutex);
		for(auto& cluster : m_clusters)
		{
			cluster->update(m_nodeInstances);
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

	void Updater::updateInstances(Engine::InstancedMesh<DischargeVertex, DischargeInstance>& mesh)
	{
		std::lock_guard<std::mutex> _(m_dischargeMutex);
		mesh.updateInstances(static_cast<UINT>(m_dischargeInstances.size()), m_dischargeInstances.size() > 0 ? &m_dischargeInstances[0] : nullptr);
	}
}