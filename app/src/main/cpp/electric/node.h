#pragma once

#include <vector>
#include "../pch.h"
#include "../engine/vector3.h"
#include "vertex.h"

namespace Electric {

	class Cluster;
	class Node;

	class DischargeListener
	{
	public:
		virtual void onDischarge(Node* pNode) = 0;
	};

	class Node
	{
	private:
		UINT m_instanceId;
		Math::Vector3 m_offset;
		Math::Vector3 m_position;
		float m_charge;
		float m_restitution;
		bool m_dirty;
		float m_dt;

		DischargeListener* m_pListener;

	public:

		Node(UINT instanceId, Math::Vector3 position, float charge, float dt,
			 DischargeListener* pListener)
				: m_instanceId(instanceId)
				, m_position(position)
				, m_offset(position)
				, m_charge(charge)
				, m_dt(dt)
				, m_dirty(true)
				, m_restitution(0.0f)
				, m_pListener(pListener)
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
			}
		}

		float getCharge() const { return m_charge; }
		void modifyCharge(float modifier) { m_charge += modifier; }

		const Math::Vector3& getPosition() const {return m_position; }
		bool resting() { return m_restitution > 0.0f; }
	};

}