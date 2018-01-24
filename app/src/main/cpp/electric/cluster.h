#pragma once

#include "node.h"
#include "../engine/path.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include <random>

namespace Electric {

	struct SearchResult
	{
		Node* pNode;
		float lengthSq;
	};

	class Cluster
	{
	private:

		typedef std::vector<Node> node_vec;

		node_vec m_nodes;
		bool m_dirty;
		Math::Vector3 m_rotation;
		Math::Vector3 m_position;
		Math::Matrix m_transform;

		struct State
		{
			typedef Engine::Path<Math::Vector3> vec3_path;
			typedef Engine::Path<float> float_path;

			vec3_path positionPath;
			vec3_path rotationPath;
			float_path minOffsetPath;
			float_path maxOffsetPath;
			vec3_path colorPath;
			float_path sizePath;
			float_path spreadYawPath;
			float_path spreadPitchPath;
		};

		std::mt19937 m_generator;

		float m_time;
		UINT m_state;

		std::vector<State> m_states;

		void toState(State& state, float transitionTime);

		State createState1();
		State createState2();

	public:

		Cluster()
			: m_time(10.0f)
			, m_state(0)
		{
		}

		void init(std::mt19937& generator, int nrNodes, const Math::Vector3& pos,
				  const Math::Vector3& rotation, std::vector<NodeInstance>& nodeInstances,
				  DischargeListener* pDischargeListener);

		void update(float dt);

		void update(std::vector<NodeInstance>& nodeInstances);

		void search(const Math::Vector3& points, float radiusSq, std::vector<SearchResult>& hits);

		const Math::Matrix& getTransform() { return m_transform; }

	};
};