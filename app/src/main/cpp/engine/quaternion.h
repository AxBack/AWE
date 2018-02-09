#pragma once

#include <math.h>
#include "../pch.h"
#include "vector3.h"
#include "matrix.h"

namespace Math {

    struct Quaternion {
        static const UINT X = 0;
        static const UINT Y = 1;
        static const UINT Z = 2;
        static const UINT W = 3;

        float m_data[4];

        Quaternion(float x, float y, float z, float w)
        {
            m_data[X] = x;
            m_data[Y] = y;
            m_data[Z] = z;
            m_data[W] = w;
        }

        Quaternion()
                :Quaternion(0,1,0,0)
        {}

        virtual Quaternion operator*(const Quaternion& rhs) const
        {
            float x1 = m_data[X];
            float y1 = m_data[Y];
            float z1 = m_data[Z];
            float w1 = m_data[W];

            float x2 = rhs.m_data[X];
            float y2 = rhs.m_data[Y];
            float z2 = rhs.m_data[Z];
            float w2 = rhs.m_data[W];

            Quaternion q;
            q.m_data[X] = x1*x2 - y1*y2 - z1*z2 - w1*w2;
            q.m_data[Y] = x1*y2 + y1*x2 - z1*w2 + w1*z2;
            q.m_data[Z] = x1*z2 + y1*w2 + z1*x2 - w1*y2;
            q.m_data[W] = x1*w2 - y1*z2 + z1*y2 + w1*x2;
        }

        virtual void operator*=(const Quaternion& rhs)
        {
            float x1 = m_data[X];
            float y1 = m_data[Y];
            float z1 = m_data[Z];
            float w1 = m_data[W];

            float x2 = rhs.m_data[X];
            float y2 = rhs.m_data[Y];
            float z2 = rhs.m_data[Z];
            float w2 = rhs.m_data[W];

            Quaternion q;
            m_data[X] = x1*x2 - y1*y2 - z1*z2 - w1*w2;
            m_data[Y] = x1*y2 + y1*x2 - z1*w2 + w1*z2;
            m_data[Z] = x1*z2 + y1*w2 + z1*x2 - w1*y2;
            m_data[W] = x1*w2 - y1*z2 + z1*y2 + w1*x2;
        }

        void normalize()
        {
            float n = 1.0f / sqrtf(m_data[X]*m_data[X] + m_data[Y]*m_data[Y] + m_data[Z]*m_data[Z] + m_data[W]*m_data[W]);
            m_data[X] *= n;
            m_data[Y] *= n;
            m_data[Z] *= n;
            m_data[W] *= n;
        }

        const float x() const { return m_data[X]; }
        const float y() const { return m_data[Y]; }
        const float z() const { return m_data[Z]; }
        const float w() const { return m_data[W]; }

        const float* data() const { return m_data; }

        static Quaternion fromEulerAngles(float x, float y, float z)
        {
            float cx = cosf(x * 0.5f);
            float sx = sinf(x * 0.5f);
            float cy = cosf(y * 0.5f);
            float sy = sinf(y * 0.5f);
            float cz = cosf(z * 0.5f);
            float sz = sinf(z * 0.5f);

            Quaternion q;
            q.m_data[X] = sx*cy*cz - cx*sy*sz;
            q.m_data[Y] = cx*sy*cz + sx*cy*sz;
            q.m_data[Z] = cx*cy*sz - sx*sy*cz;
            q.m_data[W] = cx*cy*cz + sx*sy*sz;
            return q;
        }

        static Quaternion fromAxisAngle(float x, float y, float z, float w)
        {
            return fromAxisAngle({x,y,z}, w);
        }

        static Quaternion fromAxisAngle(Vector3 axis, float degrees)
        {
            degrees = degrees * static_cast<float>(M_PI / 180.0);

            float d = sinf(degrees * 0.5f);
            axis *= d;

            Quaternion q;
            q.m_data[X] = axis.x();
            q.m_data[Y] = axis.y();
            q.m_data[Z] = axis.z();
            q.m_data[W] = cosf(degrees * 0.5f);
            return q;
        }
    };
}