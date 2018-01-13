#pragma once

#include <mutex>
#include "matrix.h"

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

		std::mutex  m_matrixMutex;
		Matrix      m_view;
		Matrix      m_projection;

	public:

		Camera()
		{
			updateView(0.5f);
			updateProjection(1, 1);
		}

		virtual ~Camera()
		{
		}

		void update()
		{
			std::lock_guard<std::mutex> _(m_matrixMutex);
			if(m_vpDirty)
			{
				m_viewProjection = m_projection * m_view;
				Vector3 normal = m_position;
				normal.normalize();
				m_right = normal.cross({0,1,0});
				m_up = m_right.cross(normal);
				m_vpDirty = false;
			}
		}

		const Matrix& getViewProjection() const { return m_viewProjection; }
		const Vector3& getUp() const { return m_up; }
		const Vector3& getRight() const { return m_right; }

		void updateView(float offset)
		{
			Matrix rotation;
			Matrix::setRotate(rotation, 0, 360.0f * (offset - 0.5f), 0);
			m_position = Matrix::transform(rotation, {0,0,-100.0f});
			{
				std::lock_guard<std::mutex> _(m_matrixMutex);
				Matrix::lookAt(m_view, m_position, {0, 0, 0}, {0, 1, 0});
				m_vpDirty = true;
			}
		}

		void updateProjection(float w, float h)
		{
			std::lock_guard<std::mutex> _(m_matrixMutex);
			Matrix::perspective(m_projection, 90.0f, w / h, 10, 1000);
			m_vpDirty = true;
		}
	};

};
