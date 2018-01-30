#pragma once

#include "node.h"
#include "../engine/path.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include <random>

namespace Electric {

	struct Search
	{
		struct Hit
		{
			Node* pNode;
			float lengthSq;
		};

		Node* pNode;
		float radiusSq;
		std::vector<Hit> hits;
	};

	class Cluster
	{
	private:

		typedef std::vector<Node> node_vec;
		typedef Engine::Path<Math::Vector3> vec3_path;
		typedef Engine::Path<float> float_path;

		struct State
		{
			vec3_path positionPath;
			vec3_path rotationPath;
			float_path offsetPath;
			float_path spreadPath;
			float_path spreadDirectionYawPath;
			float_path spreadDirectionPatchPath;
			float_path sizePath;
			vec3_path colorPath;
		};

		std::mt19937 m_generator;

		float m_time;
		UINT m_state;

		node_vec m_nodes;
		bool m_dirty;
		Math::Vector3 m_rotation;
		Math::Vector3 m_position;
		Math::Matrix m_transform;

		float m_switchInterval;
		float m_transitionTime;

		std::vector<State> m_states;

		void toState(State& state, float transitionTime);

		static void read(Engine::BinaryReader& reader, vec3_path& path);
		static void read(Engine::BinaryReader& reader, float_path& path);
		static void read(Engine::BinaryReader& reader, State& state);

	public:

		Cluster()
			: m_time(10.0f)
			, m_state(0)
		{
		}

		void init(std::mt19937& generator, Engine::BinaryReader& reader, std::vector<NodeInstance>& nodeInstances,
				  DischargeListener* pDischargeListener);

		void update(std::vector<Search>& seaches, float dt);

		void update(std::vector<NodeInstance>& nodeInstances);

		const Math::Matrix& getTransform() { return m_transform; }

	};
};