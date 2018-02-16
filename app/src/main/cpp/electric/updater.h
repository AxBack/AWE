#pragma once

#include "../engine/updater.h"
#include "cluster.h"
#include "../engine/mesh.h"

#include <random>
#include <queue>

namespace Electric {

    class Updater : public Engine::Updater, DischargeListener
    {
    private:

        struct Particle
        {
            Math::Vector3 pos;
            float size;
            Math::Color color;
        };

		struct Discharge
		{
			float time;
			float travelTime;
			Node* pStart;
			Node* pEnd;
			float charge;
		};

		typedef std::shared_ptr<Cluster> cluster_ptr;
		typedef std::vector<cluster_ptr> cluster_vec;
        typedef std::vector<Particle> particle_vec;
		typedef std::vector<NodeInstance> node_instance_vec;
		typedef std::vector<Discharge> discharge_vec;
		typedef std::vector<DischargeInstance> discharge_instance_vec;
		typedef std::queue<Search> search_queue;

		std::string m_internalFilesPath;

		std::mt19937 m_generator;
		Math::Matrix m_transform;
		bool  m_dirty;

		float m_rotation;

		std::mutex m_particleMutex;
        particle_vec m_particles;

		cluster_vec m_clusters;

		std::mutex m_nodeMutex;
		node_instance_vec m_nodeInstances;

		std::mutex m_dischargeMutex;
		discharge_vec m_discharges;
		discharge_instance_vec m_dischargeInstances;
		int m_nrDischargeInstances;
		search_queue m_dischargeSearches;
		float m_dischargeSpeed;

		void loadCluster(Engine::BinaryReader& reader);

		void updateCharges(float dt);
		void updateCluster(cluster_ptr pCluster, float dt);;
		void updateNodeInstances();

    protected:

		virtual bool init() override;
        virtual void advance(float dt) override;

    public:

		Updater()
			: m_generator(840331)
			, m_dischargeSpeed(20.0f)
			, m_dirty(true)
			, m_rotation(0.0f)
			, m_nrDischargeInstances(0)
		{
		}

		virtual bool init(const char* internalFilesPath)
		{
			m_internalFilesPath = internalFilesPath;
			return init();
		}

		void restart();
		void clear();

        void updateInstances(Engine::InstancedMesh<PositionVertex, ParticleInstance>& mesh);
		void updateInstances(Engine::InstancedMesh<PositionVertex, NodeInstance>& mesh);
		void updateInstances(Engine::InstancedMesh<DischargeVertex, DischargeInstance>& mesh);

		void onDischarge(Node* pNode, float radius) override;

    };
}
