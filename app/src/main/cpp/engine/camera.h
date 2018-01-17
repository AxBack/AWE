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
		typedef Path<Vector3> vec3_path;

		bool        m_vpDirty;
		Matrix		m_viewProjection;

		Vector3 	m_position;
		Vector3 	m_right;
		Vector3		m_up;

		std::mutex  m_matrixMutex;
		Matrix      m_view;
		Matrix      m_projection;

        //temp
        float       m_posTime;
        float       m_offset;
        vec3_path   m_positionPath;
		vec3_path	m_rotationPath;

        void updateView()
        {
            Vector3 angles = m_rotationPath.traverse(m_offset);
            Vector3 pos = m_positionPath.traverse(m_posTime);
            Matrix transform;
            Matrix::setRotate(transform, angles.x(), angles.y(), angles.z());
            m_position = Matrix::transform(transform, pos);
            Matrix::lookAt(m_view, m_position, {0, 0, 0}, {0, 1, 0});
        }

	public:

		Camera()
        : m_posTime(0.0f)
        , m_offset(0.5f)
		{

			{
				Vector3 points[] = {
						{0, 0, -1.0f},
						{0, 0, -100.0f}
				};
				m_positionPath.add(10.0f, 2, points);
			}

			{
				Vector3 points[] = {
						{0, -180.0f, 0},
						{0, 180.0f,  0}
				};
				m_rotationPath.add(1.0f, 2, points);
			}
			updateView();
			updateProjection(1, 1);
		}

		virtual ~Camera()
		{
		}

		void update(float dt)
		{

            if(m_posTime < m_positionPath.getLength())
            {
                m_posTime += dt;
                m_vpDirty = true;
            }

			std::lock_guard<std::mutex> _(m_matrixMutex);
			if(m_vpDirty)
			{
                m_vpDirty = false;
                updateView();
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

        void updateOffset(float offset)
        {
            std::lock_guard<std::mutex> _(m_matrixMutex);
            m_offset = offset;
            m_vpDirty = true;
        }

		void updateProjection(float w, float h)
		{
			std::lock_guard<std::mutex> _(m_matrixMutex);
			Matrix::perspective(m_projection, 90.0f, w / h, 10, 1000);
			m_vpDirty = true;
		}
	};

};
