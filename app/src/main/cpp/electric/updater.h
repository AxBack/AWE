#pragma once

#include <random>
#include "../engine/updater.h"
#include "../engine/vector3.h"
#include "../engine/color.h"
#include "vertex.h"
#include "../engine/matrix.h"
#include "../engine/path.h"
#include "cluster.h"

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
			Math::Vector3 start;
			Math::Vector3 end;
		};


		typedef std::shared_ptr<Cluster> cluster_ptr;
		typedef std::vector<cluster_ptr> cluster_vec;
        typedef std::vector<Particle> particle_vec;
		typedef std::vector<NodeInstance> node_instance_vec;
		typedef std::vector<Discharge> discharge_vec;

		std::mt19937 m_generator;

		std::mutex m_particleMutex;
        particle_vec m_particles;

		cluster_vec m_clusters;

		std::mutex m_nodeMutex;
		node_instance_vec m_nodeInstances;

		std::mutex m_dischargeMutex;
		discharge_vec m_charges;


		void createCluster(int nrNodes, const Math::Vector3& pos,
						  const Math::Vector3& rotation);

		void updateCharges(float dt);
		void updateCluster(cluster_ptr pCluster, float dt);;
		void updateNodeInstances();

    protected:

        virtual void advance(float dt) override;

    public:

		Updater()
			: m_generator(840331)
		{
		}

		virtual bool init() override;

        void updateInstances(Engine::InstancedMesh<PositionVertex, ParticleInstance>& mesh);
		void updateInstances(Engine::InstancedMesh<PositionVertex, NodeInstance>& mesh);
		void updateInstances(Engine::InstancedMesh<ChargeVertex, ChargeInstance>& mesh);

		void onDischarge(Node* pNode) override;

    };
}
