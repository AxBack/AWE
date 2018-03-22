#pragma once

#include "node.h"
#include "../engine/path.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include <random>

namespace Electric {

	struct Search
	{
		Node* pNode;
		float radiusSq;
		Node* pHit;
		float value;
	};

	class Cluster
	{
	public:
		typedef std::shared_ptr<Node> node_ptr;
		typedef std::vector<node_ptr> node_vec;
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

	private:

		std::mt19937 m_generator;

		float m_time;
		UINT m_state;

		node_vec m_nodes;
		bool m_dirty;
		Math::Vector3 m_rotation;
		Math::Vector3 m_position;
		Math::Matrix m_transform;

		float_path m_chargePath;
		int m_nrNodes;

		float m_switchInterval;
		float m_transitionTime;

		std::vector<State> m_states;

		node_ptr createNode(DischargeListener* pDischargeListener);
		void toState(const State& state, float transitionTime);

		static void read(Engine::BinaryReader& reader, vec3_path& path, const Math::Vector3 def = {0,0,0});
		static void read(Engine::BinaryReader& reader, float_path& path, const float def = 0.0f);
		static void read(Engine::BinaryReader& reader, State& state);

	public:

		Cluster()
			: m_time(0.0f)
			, m_state(0)
			, m_nrNodes(0)
			, m_dirty(true)
			, m_switchInterval(10.0f)
			, m_transitionTime(0.0f)
		{
		}

		void init(std::mt19937& generator, Engine::BinaryReader& reader, DischargeListener* pDischargeListener);

		void read(Engine::BinaryReader& reader, DischargeListener* pDischargeListener);

		void setState(const State& state);

		void update(const Math::Matrix& parent, bool dirty, std::vector<Search>& searches, float dt);

		void update(std::vector<NodeInstance>& nodeInstances, UINT& currentIndex);

		const Math::Matrix& getTransform() { return m_transform; }

	};
};