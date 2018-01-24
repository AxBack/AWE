#include "node.h"
#include "cluster.h"

namespace Electric {

	float OSMOSIS_PER_SECOND = 0.05f;
	float CHARGE_FLOW_FACTOR = 0.025f;

	void Node::update(Cluster* pOwner, bool dirty, float dt)
	{
		m_transitionTime += dt;
		if(m_pOffsetPath != nullptr)
		{
			m_offset = m_pOffsetPath->traverse(m_transitionTime);
			m_dirty = true;
			if(m_transitionTime > m_pOffsetPath->getLength())
				m_pOffsetPath = nullptr;
		}

		if(m_pColorPath != nullptr)
		{
			m_color = m_pColorPath->traverse(m_transitionTime);
			m_dirty = true;
			if(m_transitionTime > m_pColorPath->getLength())
				m_pColorPath = nullptr;
		}

		if(m_pScalePath!= nullptr)
		{
			m_scale = m_pScalePath->traverse(m_transitionTime);
			m_dirty = true;
			if(m_transitionTime > m_pScalePath->getLength())
				m_pScalePath = nullptr;
		}

		if(dirty)
		{
			m_dirty = dirty;
			m_position = pOwner->getTransform().transform(m_offset);
		}

		float flow = CHARGE_FLOW_FACTOR * dt;
		m_charge += dt * OSMOSIS_PER_SECOND;

		if(m_restitution > 0.0f)
			m_restitution -= dt;

		float f = std::min(flow, m_charge);
		m_charge -= f;

		if(m_charge > 1.0f)
		{
			m_restitution = 1.0f;
			m_pListener->onDischarge(this);
		}
	}

}
