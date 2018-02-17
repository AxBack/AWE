#pragma once

#include <vector>
#include "../pch.h"
#include "../engine/vector3.h"
#include "vertex.h"
#include "../engine/color.h"
#include "../engine/path.h"

namespace Electric {

	class Cluster;
	class Node;

	class DischargeListener
	{
	public:
		virtual void onDischarge(Node* pNode, float radius) = 0;
	};

	class Node
	{
	private:

		typedef Engine::Path<Math::Vector3> vec3_path;
		typedef Engine::Path<float> float_path;
		typedef std::shared_ptr<vec3_path> vec3_path_ptr;
		typedef std::shared_ptr<float_path> float_path_ptr;

		UINT m_instanceId;

		Math::Vector3 m_offset;
		Math::Vector3 m_position;
		Math::Vector3 m_color;
		float m_size;
		float m_charge;
		float m_restitution;
		bool m_dirty;
		float m_delta;

		float m_transitionTime;
		vec3_path_ptr m_pOffsetPath;
		vec3_path_ptr m_pColorPath;
		float_path_ptr m_pSizePath;

		DischargeListener* m_pListener;

		bool m_waitingForDischargeResult;

	public:

		Node(UINT instanceId, Math::Vector3 position, Math::Vector3 color, float size, float charge,
			 float dt, DischargeListener* pListener)
				: m_instanceId(instanceId)
				, m_position(position)
				, m_offset(position)
				, m_color(color)
				, m_size(size)
				, m_charge(charge)
				, m_delta(dt)
				, m_dirty(true)
				, m_restitution(0.0f)
				, m_pListener(pListener)
				, m_transitionTime(0.0f)
				, m_waitingForDischargeResult(false)
		{
		}

		void update(Cluster* pOwner, bool dirty, float dt);
		void update(std::vector<NodeInstance>& nodeInstances)
		{
			NodeInstance* pInstance = &nodeInstances[m_instanceId];
			pInstance->charge = m_charge;
			if(m_dirty)
			{
				m_dirty = false;
				pInstance->x = m_position.x();
				pInstance->y = m_position.y();
				pInstance->z = m_position.z();
				pInstance->size = m_size;
				pInstance->r = m_color.x();
				pInstance->g = m_color.y();
				pInstance->b = m_color.z();
			}
		}

		void transition(vec3_path_ptr pOffsetPath, vec3_path_ptr pColorPath, float_path_ptr pScalePath)
		{
			m_transitionTime = 0;
			m_pOffsetPath = pOffsetPath;
			m_pColorPath = pColorPath;
			m_pSizePath = pScalePath;
		}

		void onDischargeResult(Node* pNode);

		float getCharge() const { return m_charge; }
		void modifyCharge(float modifier) { m_charge += modifier; }

		const Math::Vector3& getPosition() const {return m_position; }
		const Math::Vector3& getOffset() const { return m_offset; }
		const Math::Vector3& getColor() const {return m_color; }
		float getScale() const { return m_size; }
		bool resting() { return m_restitution > 0.0f; }

		float getDelta() { return m_delta; }
	};

}