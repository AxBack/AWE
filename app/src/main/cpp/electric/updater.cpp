#include "updater.h"
#include "../engine/binary_reader.h"

#include <algorithm>
#include <bits/ios_base.h>
#include <fstream>

namespace Electric {

float DISCHARGE_FACTOR = 1.0f;
float LOSS_FACTOR = 0.5f;

	bool Updater::init()
	{
		Math::Matrix::identity(m_transform);

		Engine::BinaryReader reader((m_internalFilesPath + "/WAE.dat").c_str());
		int nrClusters = reader.read<int>();

		for(int i=0; i<nrClusters; ++i)
		{
			loadCluster(reader);
		}

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
		for(m_nrDischargeInstances=0; m_nrDischargeInstances<m_discharges.size();)
		{
			Discharge& discharge = m_discharges[m_nrDischargeInstances];
			discharge.time -= dt;
			discharge.randChangeTimer -= dt;

			if(discharge.randChangeTimer <= 0.0f)
			{
				discharge.rand += dist(m_generator);
				discharge.randChangeTimer = 0.03f;
			}

			if(discharge.time <= 0.0f)
			{
				discharge.pEnd->modifyCharge( discharge.charge * LOSS_FACTOR );
				m_discharges.erase(m_discharges.begin() + m_nrDischargeInstances);
			}
			else
			{
				const Math::Vector3& s = discharge.pStart->getPosition();
				const Math::Vector3& d = discharge.pEnd->getPosition();
				const Math::Vector3& c = discharge.pStart->getColor();

				if(m_nrDischargeInstances >= m_dischargeInstances.size())
					m_dischargeInstances.push_back({s.x(), s.y(), s.z(), d.x(), d.y(), d.z(), c.x(), c.y(), c.z(), discharge.rand});
				else
				{
					m_dischargeInstances[m_nrDischargeInstances].sx = s.x();
					m_dischargeInstances[m_nrDischargeInstances].sy = s.y();
					m_dischargeInstances[m_nrDischargeInstances].sz = s.z();

					m_dischargeInstances[m_nrDischargeInstances].dx = d.x();
					m_dischargeInstances[m_nrDischargeInstances].dy = d.y();
					m_dischargeInstances[m_nrDischargeInstances].dz = d.z();

					m_dischargeInstances[m_nrDischargeInstances].r = c.x();
					m_dischargeInstances[m_nrDischargeInstances].g = c.y();
					m_dischargeInstances[m_nrDischargeInstances].b = c.z();

					m_dischargeInstances[m_nrDischargeInstances].rand = discharge.rand;
				}
				++m_nrDischargeInstances;
			}
		}
	}

	void Updater::updateCluster(cluster_ptr pCluster, float dt)
	{
		std::vector<Search> searches;
		while(m_dischargeSearches.size() > 0 && searches.size() < 5)
		{
			Search s = m_dischargeSearches.front();
			searches.push_back(s);
			m_dischargeSearches.pop();
		}

		pCluster->update(m_transform, m_dirty, searches, dt);

		for(auto& it : searches)
		{
			if(it.pHit && !it.pHit->resting())
			{
				float f = it.pNode->getCharge() * DISCHARGE_FACTOR;
				it.pNode->modifyCharge(-f);

				std::uniform_real_distribution<float> dist(0.0f, 1.0f);

				std::lock_guard<std::mutex> _(m_dischargeMutex);
				m_discharges.push_back({m_dischargeTime, it.pNode, it.pHit, f, dist(m_generator)});
			}

			it.pNode->onDischargeResult(it.pHit);
		}
	}

	void Updater::onDischarge(Node* pNode, float radius)
	{
		m_dischargeSearches.push({pNode, radius * radius, nullptr, FLT_MAX});
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
		mesh.updateInstances(m_nrDischargeInstances, m_nrDischargeInstances > 0 ? &m_dischargeInstances[0] : nullptr);
	}
}