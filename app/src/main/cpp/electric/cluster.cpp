#include "cluster.h"
#include "../engine/matrix.h"
#include "vertex.h"

namespace Electric {

	void Cluster::init(std::mt19937& generator, int nrNodes, const Math::Vector3& pos,
					   const Math::Vector3& rotation, std::vector<NodeInstance>& nodeInstances,
					   DischargeListener* pDischargeListener)
	{
		Cluster::State state;
		{
			Math::Vector3 points[] = {
					{0,150,0},
					{0,-150,0}
			};
			state.positionPath.add(1.0f, 2, points);
		}

		{
			Math::Vector3 points[] = {
					{0,-90,0},
					{0,90,0}
			};
			state.rotationPath.add(1.0f, 2, points);
		}

		{
			float points[] = {-100};
			state.minOffsetPath.add(1.0f, 1, points);
		}

		{
			float points[] = {100};
			state.maxOffsetPath.add(1.0f, 1, points);
		}

		{
			float points[] = {0,1};
			state.chargePath.add(1.0f, 2, points);
		}

		{
			Math::Vector3 points[] = {
					{0,0,1},
					{1,0,0},
			};
			state.colorPath.add(1.0f, 2, points);
		}

		{
			float points[] = {1,2};
			state.sizePath.add(1.0f, 2, points);
		}

		{
			float points[] = {45};
			state.spreadYawPath.add(1.0f, 1, points);
		}

		{
			float points[] = {45};
			state.spreadPitchPath.add(1.0f, 1, points);
		}

		m_dirty = true;
		m_position = pos;
		m_rotation = rotation;

		std::uniform_real_distribution<> dist(0.0f, 1.0f);

		for(int i = 0; i < nrNodes; ++i)
		{
			float d = static_cast<float>(dist(generator));

			Math::Matrix rot;
			{
				Math::Vector3 r = state.rotationPath.traverse(d);
				Math::Matrix::setRotate(rot, r.x(), r.y(), r.z());
			}

			float yaw = state.spreadYawPath.traverse(d);
			std::uniform_real_distribution<> yawDist(-yaw, yaw);
			yaw = static_cast<float>(yawDist(generator));

			float pitch = state.spreadPitchPath.traverse(d);
			std::uniform_real_distribution<> pitchDist(-pitch, pitch);
			pitch = static_cast<float>(pitchDist(generator));

			float a = static_cast<float>(dist(generator)) * 2.0f * static_cast<float>(M_PI);
			float r = static_cast<float>(sqrt(dist(generator)));

			yaw = r * static_cast<float>(cos(a)) * yaw;
			pitch = r * static_cast<float>(sin(a)) * pitch;

			Math::Matrix rotSpread;
			Math::Matrix::setRotate(rotSpread, yaw, pitch, 0);
			rotSpread = rotSpread * rot;

			Math::Vector3 p = rotSpread.transform({0,0,-1});

			float minOffset = state.minOffsetPath.traverse(d);
			float maxOffset = state.maxOffsetPath.traverse(d);
			std::uniform_real_distribution<> offsetDist(minOffset, maxOffset);

			p *= static_cast<float>(offsetDist(generator));
			p += state.positionPath.traverse(d);

			float s = state.sizePath.traverse(d);
			float c = state.chargePath.traverse(static_cast<float>(dist(generator)));

			Math::Vector3 color = state.colorPath.traverse(d);
			m_nodes.push_back(Node(static_cast<UINT>(nodeInstances.size()), p, c, d, pDischargeListener));
			nodeInstances.push_back({p.x(), p.y(), p.z(), s, c, color.x(), color.y(), color.z()});
		}
	}

	void Cluster::update(float dt)
	{
		bool dirty = m_dirty;
		if(m_dirty)
		{
			Math::Matrix::setRotate(m_transform,
									m_rotation.x(),
									m_rotation.y(),
									m_rotation.z());

			Math::Matrix::translate(m_transform,
									m_position.x(),
									m_position.y(),
									m_position.z());
		}

		for(auto& it : m_nodes)
		{
			it.update(this, dirty, dt);
		}
	}

	void Cluster::update(std::vector<NodeInstance>& nodeInstances)
	{
		for(auto& node : m_nodes)
		{
			node.update(nodeInstances);
		}
	}

	void Cluster::search(const Math::Vector3& point, float radiusSq, std::vector<SearchResult>& hits)
	{
		for(auto& node : m_nodes)
		{
			if(!node.resting())
			{
				float l2 = (node.getPosition() - point).lengthSq();
				if(l2 <=  radiusSq)
					hits.push_back({ &node, l2 });
			}
		}
	}

}
