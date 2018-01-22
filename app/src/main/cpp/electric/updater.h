#pragma once

#include <random>
#include "../engine/updater.h"
#include "../engine/vector3.h"
#include "../engine/color.h"
#include "vertex.h"
#include "../engine/matrix.h"
#include "../engine/path.h"

namespace Electric {

    class Updater : public Engine::Updater
    {
    private:

        struct Particle
        {
            Math::Vector3 pos;
            float size;
            Math::Color color;
        };

		struct Node;

		struct Connection
		{
			Node* pNode;
			float lengthSq;
		};

		struct Node
		{
			UINT instanceId;
			Math::Vector3 offset;
			Math::Vector3 position;
			float charge;
			float restitution;
			bool dirty;
			std::vector<Connection> connections;
		};

		typedef std::vector<Node> node_vec;

		struct Cluster
		{
			node_vec nodes;
			bool dirty;
			Math::Vector3 rotation;
			Math::Vector3 position;
			Math::Matrix transform;
		};

		struct Discharge
		{
			float time;
			Math::Vector3 start;
			Math::Vector3 end;
		};

		struct ClusterCreator
		{
			typedef Engine::Path<Math::Vector3> vec3_path;
			typedef Engine::Path<float> float_path;

			vec3_path positionPath;
			float_path offsetPath;
			float_path chargePath;
			vec3_path colorPath;
			float_path sizePath;
		};

		struct SearchResult
		{
			Node* pNode;
			float lengthSq;
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


		void setupCluster(int nrNodes, const Math::Vector3& pos,
						  const Math::Vector3& rotation, ClusterCreator& cc);
		void connectNodes(int nrConnectionsPerNode);

		void updateCharges(float dt);
		void updateCluster(cluster_ptr pCluster, float dt);
		void updateNode(Node* pNode, float dt);
		void discharge(Node* pNode);
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

    };
}
