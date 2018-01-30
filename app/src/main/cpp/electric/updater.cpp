#include "updater.h"
#include "../engine/binary_reader.h"

#include <algorithm>
#include <bits/ios_base.h>
#include <fstream>

namespace Electric {

float DISCHARGE_FACTOR = 1.0f;
float LOSS_FACTOR = 0.25f;

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
		for(auto it = m_discharges.begin(); it != m_discharges.end(); )
		{
			it->time -= dt;
			it->randChangeTimer -= dt;

			if(it->randChangeTimer <= 0.0f)
			{
				it->rand = dist(m_generator);
				it->randChangeTimer = 0.03f;
			}

			if(it->time <= 0.0f)
				it = m_discharges.erase(it);
			else
				++it;
		}
	}

	void Updater::updateCluster(cluster_ptr pCluster, float dt)
	{
		std::vector<Search> searches;
		while(m_dischargeSearches.size() > 0 && searches.size() < 5)
		{
			Search s = m_dischargeSearches.back();
			searches.push_back(s);
			m_dischargeSearches.pop();
		}

		pCluster->update(searches, dt);

		for(auto& it : searches)
		{
			Search::Hit* p = nullptr;
			if(it.hits.size() == 0)
				continue;

			float currMin = FLT_MAX;
			for(int i=0; i<it.hits.size(); ++i)
			{
				float v = (it.hits[i].lengthSq / it.radiusSq) + (it.pNode->getCharge() - it.hits[i].pNode->getCharge());
				if(currMin > v)
				{
					p = &it.hits[i];
					currMin = v;
				}
			}

			if(p)
			{
				float f = it.pNode->getCharge() * DISCHARGE_FACTOR;
				p->pNode->modifyCharge( f * LOSS_FACTOR );
				it.pNode->modifyCharge(-f);
				it.pNode->onDischargeResult(p->pNode);

				std::uniform_real_distribution<float> dist(0.0f, 1.0f);

				std::lock_guard<std::mutex> _(m_dischargeMutex);
				m_discharges.push_back({m_dischargeTime, it.pNode, p->pNode, dist(m_generator)});
			}
		}
	}

	void Updater::onDischarge(Node* pNode, float radius)
	{
		m_dischargeSearches.push({pNode, radius * radius});
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
		for(auto& it : m_clusters)
			updateCluster(it, dt);
		updateCharges(dt);
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
		if(m_dischargeInstances .size() < m_discharges.size())
			m_dischargeInstances.resize(m_discharges.size());

		UINT i=0;
		for(auto& it : m_discharges)
		{
			Math::Vector3 s = it.pStart->getPosition();
			Math::Vector3 d = it.pEnd->getPosition();
			Math::Vector3 c = it.pStart->getColor();

			m_dischargeInstances[i].sx = s.x();
			m_dischargeInstances[i].sy = s.y();
			m_dischargeInstances[i].sz = s.z();

			m_dischargeInstances[i].dx = d.x();
			m_dischargeInstances[i].dy = d.y();
			m_dischargeInstances[i].dz = d.z();

			m_dischargeInstances[i].r = c.x();
			m_dischargeInstances[i].g = c.y();
			m_dischargeInstances[i].b = c.z();

			m_dischargeInstances[i].rand = it.rand;
			++i;
		}
		mesh.updateInstances(static_cast<UINT>(m_discharges.size()), m_discharges.size() > 0 ? &m_dischargeInstances[0] : nullptr);
	}
}