#pragma once

#include <mutex>
#include "matrix.h"
#include "path.h"

namespace Engine {

	class Camera
	{
	private:

		typedef Math::Matrix Matrix;
		typedef Math::Vector3 Vector3;

		bool        m_vpDirty;
		Matrix		m_viewProjection;

		Vector3 	m_position;
		Vector3 	m_at;
		Vector3 	m_forward;
		Vector3 	m_right;
		Vector3		m_up;

		Matrix      m_view;
		Matrix      m_projection;

	public:

		Camera()
        : m_vpDirty(true)
        , m_position(0,0,0)
        , m_right(1,0,0)
        , m_up(0,1,0)
		{
		}

		virtual ~Camera()
		{
		}

		void update()
		{
			if(m_vpDirty)
			{
                m_vpDirty = false;
				m_viewProjection = m_projection * m_view;
				m_forward = m_at - m_position;
				m_forward.normalize();
				m_right = m_forward.cross(m_up);
			}
		}

		const Matrix& getViewProjection() const { return m_viewProjection; }
		const Vector3& getPosition() const { return m_position; }
		const Vector3& getForward() const { return m_forward; }
		const Vector3& getUp() const { return m_up; }
		const Vector3& getRight() const { return m_right; }

        void updateView(const Vector3& position, const Vector3& at, const Vector3& up)
        {
            m_position = position;
			m_up = up;
			m_at = at;
            Matrix::lookAt(m_view, m_position, at, up);
            m_vpDirty = true;
        }

		void updateProjection(float w, float h)
		{
			Matrix::perspective(m_projection, 90.0f, w / h, 1.0f, 1000.0f);
			m_vpDirty = true;
		}
	};

};
