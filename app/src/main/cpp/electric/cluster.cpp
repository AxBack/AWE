#include "cluster.h"
#include "../engine/matrix.h"
#include "vertex.h"
#include "engine.h"

namespace Electric {

	void Cluster::init(std::mt19937& generator, Engine::BinaryReader& reader,
					   std::vector<NodeInstance>& nodeInstances, DischargeListener* pDischargeListener)
	{
		m_generator = generator;
		m_dirty = true;
		m_position.read(reader);
		m_rotation.read(reader);

		std::uniform_real_distribution<> dist(0.0f, 1.0f);

		float_path chargePath;
		read(reader, chargePath);

		int nrNodes = reader.read<int>();
		for(UINT i=0; i<nrNodes; ++i)
		{
			float dt = static_cast<float>(dist(generator));
			float charge = chargePath.traverse(static_cast<float>(dist(generator)));

			m_nodes.push_back(Node(static_cast<UINT>(nodeInstances.size()), {0,0,0}, {0,0,0}, 1.0f,
								   charge, dt, pDischargeListener));
			nodeInstances.push_back({0, 0, 0, 0.0f, charge, 0,0,0});
		}

		int nrStates = reader.read<int>();
		for(int i=0; i<nrStates; ++i)
		{
			State state;
			read(reader, state);
			m_states.push_back(state);
		}

		toState(m_states[0], 5.0f);
	}

	void Cluster::read(Engine::BinaryReader& reader, vec3_path& path)
	{
		int nrParts = reader.read<int>();
		for(int i=0; i<nrParts; ++i)
		{
			float time = reader.read<float>();
			int nrPoints = reader.read<int>();
			std::unique_ptr<Math::Vector3[]> p(new Math::Vector3[nrPoints]);
			for(int j=0; j<nrPoints; ++j)
				p[j].read(reader);
			path.add(time, static_cast<UINT>(nrPoints), p.get());
		}
	}

	void Cluster::read(Engine::BinaryReader& reader, float_path& path)
	{
		int nrParts = reader.read<int>();
		for(int i=0; i<nrParts; ++i)
		{
			float time = reader.read<float>();
			int nrPoints = reader.read<int>();
			std::unique_ptr<float[]> p(new float[nrPoints]);
			for(int j=0; j<nrPoints; ++j)
				p[j] = reader.read<float>();
			path.add(time, static_cast<UINT>(nrPoints), p.get());
		}
	}

	void Cluster::read(Engine::BinaryReader& reader, State& state)
	{
		read(reader, state.positionPath);
		read(reader, state.rotationPath);
		read(reader, state.minOffsetPath);
		read(reader, state.maxOffsetPath);
		read(reader, state.colorPath);
		read(reader, state.sizePath);
		read(reader, state.spreadYawPath);
		read(reader, state.spreadPitchPath);
	}

	void Cluster::toState(State& state, float transitionTime)
	{
		std::uniform_real_distribution<> dist(0.0f, 1.0f);
		for(auto& it : m_nodes)
		{
			float d = it.getDelta();
			Math::Matrix rot;
			{
				Math::Vector3 r = state.rotationPath.traverse(d);
				Math::Matrix::setRotate(rot, r.x(), r.y(), r.z());
			}

			float yaw = state.spreadYawPath.traverse(d);
			std::uniform_real_distribution<> yawDist(-yaw, yaw);
			yaw = static_cast<float>(yawDist(m_generator));

			float pitch = state.spreadPitchPath.traverse(d);
			std::uniform_real_distribution<> pitchDist(-pitch, pitch);
			pitch = static_cast<float>(pitchDist(m_generator));

			float a = static_cast<float>(dist(m_generator)) * 2.0f * static_cast<float>(M_PI);
			float r = static_cast<float>(sqrt(dist(m_generator)));

			yaw = r * static_cast<float>(cos(a)) * yaw;
			pitch = r * static_cast<float>(sin(a)) * pitch;

			Math::Matrix rotSpread;
			Math::Matrix::setRotate(rotSpread, yaw, pitch, 0);
			rotSpread = rotSpread * rot;

			Math::Vector3 p = rotSpread.transform({0,0,-1});

			float minOffset = state.minOffsetPath.traverse(d);
			float maxOffset = state.maxOffsetPath.traverse(d);
			std::uniform_real_distribution<> offsetDist(minOffset, maxOffset);

			p *= static_cast<float>(offsetDist(m_generator));
			p += state.positionPath.traverse(d);

			float s = state.sizePath.traverse(d);

			Math::Vector3 color = state.colorPath.traverse(d);

			std::shared_ptr<vec3_path> offsetPath(new vec3_path());
			{
				Math::Vector3 start = it.getOffset();
				Math::Vector3 diff = p - start;
				Math::Vector3 points[] = {start,
									  start + (diff * 0.95f),
									  start + (diff * 0.99f),
									  p
				};
				offsetPath->add(transitionTime, 4, points);
			};

			std::shared_ptr<vec3_path> colorPath(new vec3_path());
			{
				Math::Vector3 points[] = {it.getColor(), color};
				colorPath->add(transitionTime, 2, points);
			};

			std::shared_ptr<float_path> scalePath(new float_path());
			{
				float points[] = {it.getScale(), s };
				scalePath->add(transitionTime, 2, points);
			};

			it.transition(offsetPath, colorPath, scalePath);
		}
	}

	void Cluster::update(float dt)
	{
		m_time -= dt;
		if(m_time <= 0.0f)
		{
			m_state = ++m_state % static_cast<UINT>(m_states.size());
			m_time = 10.0f;
			toState(m_states[m_state], 5.0f);
		}

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
