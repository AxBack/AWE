#pragma once

#include <random>
#include "../engine/updater.h"
#include "../engine/vector3.h"
#include "../engine/color.h"
#include "vertex.h"

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

		struct Node
		{
			Math::Vector3 pos;
			float charge;
			std::vector<UINT> connections;
		};

        typedef std::vector<Particle> particle_vec;

		typedef std::vector<Node> node_vec;
		typedef std::vector<NodeInstance> node_instance_vec;

		std::mt19937 m_generator;

		std::mutex m_particleMutex;
        particle_vec m_particles;

		std::mutex m_nodeMutex;
		node_vec m_nodes;
		node_instance_vec m_nodeInstances;

    protected:

        virtual void advance(float dt) override;

    public:

		Updater()
				: m_generator(840331)
		{
		}

		virtual bool init() override;

        void updateInstances(Engine::Mesh<Vertex, ParticleInstance>& mesh);
		void updateInstances(Engine::Mesh<Vertex, NodeInstance>& mesh);

    };
}
