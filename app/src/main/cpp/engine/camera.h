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
				Vector3 normal = m_position;
				normal.normalize();
				m_right = normal.cross({0,1,0});
				m_up = m_right.cross(normal);
			}
		}

		const Matrix& getViewProjection() const { return m_viewProjection; }
		const Vector3& getUp() const { return m_up; }
		const Vector3& getRight() const { return m_right; }

        void updateView(const Vector3& position, const Vector3& at, const Vector3& up)
        {
            m_position = position;
            Matrix::lookAt(m_view, m_position, at, up);
            m_vpDirty = true;
        }

		void updateProjection(float w, float h)
		{
			Matrix::perspective(m_projection, 90.0f, w / h, 1, 1000);
			m_vpDirty = true;
		}
	};

};
