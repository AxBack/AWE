#pragma once
//
// Created by AxB on 4/23/2017.
//

#include <string.h>
#include "vector3.h"
namespace Math {

	class Matrix {
	private:

#define M00 0
#define M10 1
#define M20 2
#define M30 3
#define M01 4
#define M11 5
#define M21 6
#define M31 7
#define M02 8
#define M12 9
#define M22 10
#define M32 11
#define M03 12
#define M13 13
#define M23 14
#define M33 15

		float m_data[16];

	public:

		Matrix() {
		}

		const float *const data() const { return m_data; }

		Matrix operator*(const Matrix &rhs) {
			return multiply(*this, rhs);
		}

		float &operator[](const int index) {
			return m_data[index];
		}

		float operator[](const int index) const {
			return m_data[index];
		}

		Vector3 transform(const Vector3 &point) {
			return Matrix::transform(*this, point);
		}

		Vector3 project(const Vector3 &point) {
			return Matrix::project(*this, point);
		}

		static void identity(Matrix &matrix) {
			memset(matrix.m_data, 0, sizeof(matrix));
			matrix[M00] = matrix[M11] = matrix[M22] = matrix[M33] = 1.0f;
		}

		static Matrix multiply(const Matrix &left, const Matrix &right) {
			Matrix ret;

			const float *pL = left.m_data;
			const float *pR = right.m_data;

			ret[M00] =
					pR[M00] * pL[M00] + pR[M10] * pL[M01] + pR[M20] * pL[M02] + pR[M30] * pL[M03];
			ret[M10] =
					pR[M00] * pL[M10] + pR[M10] * pL[M11] + pR[M20] * pL[M12] + pR[M30] * pL[M13];
			ret[M20] =
					pR[M00] * pL[M20] + pR[M10] * pL[M21] + pR[M20] * pL[M22] + pR[M30] * pL[M23];
			ret[M30] =
					pR[M00] * pL[M30] + pR[M10] * pL[M31] + pR[M20] * pL[M32] + pR[M30] * pL[M33];

			ret[M01] =
					pR[M01] * pL[M00] + pR[M11] * pL[M01] + pR[M21] * pL[M02] + pR[M31] * pL[M03];
			ret[M11] =
					pR[M01] * pL[M10] + pR[M11] * pL[M11] + pR[M21] * pL[M12] + pR[M31] * pL[M13];
			ret[M21] =
					pR[M01] * pL[M20] + pR[M11] * pL[M21] + pR[M21] * pL[M22] + pR[M31] * pL[M23];
			ret[M31] =
					pR[M01] * pL[M30] + pR[M11] * pL[M31] + pR[M21] * pL[M32] + pR[M31] * pL[M33];

			ret[M02] =
					pR[M02] * pL[M00] + pR[M12] * pL[M01] + pR[M22] * pL[M02] + pR[M32] * pL[M03];
			ret[M12] =
					pR[M02] * pL[M10] + pR[M12] * pL[M11] + pR[M22] * pL[M12] + pR[M32] * pL[M13];
			ret[M22] =
					pR[M02] * pL[M20] + pR[M12] * pL[M21] + pR[M22] * pL[M22] + pR[M32] * pL[M23];
			ret[M32] =
					pR[M02] * pL[M30] + pR[M12] * pL[M31] + pR[M22] * pL[M32] + pR[M32] * pL[M33];

			ret[M03] =
					pR[M03] * pL[M00] + pR[M13] * pL[M01] + pR[M23] * pL[M02] + pR[M33] * pL[M03];
			ret[M13] =
					pR[M03] * pL[M10] + pR[M13] * pL[M11] + pR[M23] * pL[M12] + pR[M33] * pL[M13];
			ret[M23] =
					pR[M03] * pL[M20] + pR[M13] * pL[M21] + pR[M23] * pL[M22] + pR[M33] * pL[M23];
			ret[M33] =
					pR[M03] * pL[M30] + pR[M13] * pL[M31] + pR[M23] * pL[M32] + pR[M33] * pL[M33];

			return ret;
		}

		static Matrix &perspective(Matrix &m, float fov, float aspect, float zNear, float zFar) {
			float fh = tanf(fov / 360.0f * static_cast<float>(M_PI)) * zNear;
			float fw = fh * aspect;

			return frustum(m, -fw, fw, -fh, fh, zNear, zFar);
		}

		static Matrix &
		frustum(Matrix &m, float left, float right, float bottom, float top, float near,
				float far) {
			float width = 1.0f / (right - left);
			float height = 1.0f / (top - bottom);
			float depth = 1.0f / (near - far);
			float x = 2.0f * (near * width);
			float y = 2.0f * (near * height);
			float a = (right + left) * width;
			float b = (top + bottom) * height;
			float c = (far + near) * depth;
			float d = 2.0f * (far * near * depth);

			m[M00] = x;
			m[M11] = y;
			m[M02] = a;
			m[M12] = b;
			m[M22] = c;
			m[M23] = d;
			m[M32] = -1.0f;
			m[M10] = 0.0f;
			m[M20] = 0.0f;
			m[M30] = 0.0f;
			m[M01] = 0.0f;
			m[M21] = 0.0f;
			m[M31] = 0.0f;
			m[M03] = 0.0f;
			m[M13] = 0.0f;
			m[M33] = 0.0f;

			return m;
		}

		static Matrix &
		ortho(Matrix &m, float left, float right, float bottom, float top, float near,
			  float far) {
			m[M00] = 2.0f / (right - left);
			m[M10] = 0.0f;
			m[M20] = 0.0f;
			m[M30] = 0.0f;
			m[M01] = 0.0f;
			m[M11] = 2.0f / (top - bottom);
			m[M21] = 0.0f;
			m[M31] = 0.0f;
			m[M02] = 0.0f;
			m[M12] = 0.0f;
			m[M22] = 2.0f / (far - near);
			m[M32] = 0.0f;
			m[M03] = -(right + left) / (right - left);
			m[M13] = -(top + bottom) / (top - bottom);
			m[M23] = -(near + far) / (far - near);
			m[M33] = 1.0f;

			return m;
		}

		static Matrix &lookAt(Matrix &m, const Vector3 &eye, const Vector3 &at, const Vector3 &up) {
			Vector3 f = at - eye;
			f.normalize();

			Vector3 s = f.cross(up);
			s.normalize();

			Vector3 u = s.cross(f);

			m[M00] = s.x();
			m[M10] = u.x();
			m[M20] = -f.x();
			m[M30] = 0.0f;
			m[M01] = s.y();
			m[M11] = u.y();
			m[M21] = -f.y();
			m[M31] = 0.0f;
			m[M02] = s.z();
			m[M12] = u.z();
			m[M22] = -f.z();
			m[M32] = 0.0f;
			m[M03] = 0.0f;
			m[M13] = 0.0f;
			m[M23] = 0.0f;
			m[M33] = 1.0f;

			return translate(m, -eye.x(), -eye.y(), -eye.z());
		}

		static float determinant(const Matrix &m) {
			return m[M30] * m[M21] * m[M12] * m[M03] -
				   m[M20] * m[M31] * m[M12] * m[M03] -
				   m[M30] * m[M11] * m[M22] * m[M03] +
				   m[M10] * m[M31] * m[M22] * m[M03] +
				   m[M20] * m[M11] * m[M32] * m[M03] -
				   m[M10] * m[M21] * m[M32] * m[M03] -
				   m[M30] * m[M21] * m[M02] * m[M13] +
				   m[M20] * m[M31] * m[M02] * m[M13] +
				   m[M30] * m[M01] * m[M22] * m[M13] -
				   m[M00] * m[M31] * m[M22] * m[M13] -
				   m[M20] * m[M01] * m[M32] * m[M13] +
				   m[M00] * m[M21] * m[M32] * m[M13] +
				   m[M30] * m[M11] * m[M02] * m[M23] -
				   m[M10] * m[M31] * m[M02] * m[M23] -
				   m[M30] * m[M01] * m[M12] * m[M23] +
				   m[M00] * m[M31] * m[M12] * m[M23] +
				   m[M10] * m[M01] * m[M32] * m[M23] -
				   m[M00] * m[M11] * m[M32] * m[M23] -
				   m[M20] * m[M11] * m[M02] * m[M33] +
				   m[M10] * m[M21] * m[M02] * m[M33] +
				   m[M20] * m[M01] * m[M12] * m[M33] -
				   m[M00] * m[M21] * m[M12] * m[M33] -
				   m[M10] * m[M01] * m[M22] * m[M33] +
				   m[M00] * m[M11] * m[M22] * m[M33];
		}

        static inline Matrix& setRotate(Matrix& m, float x, float y, float z)
        {
            x *= static_cast<float>(M_PI) / 180.0f;
            y *= static_cast<float>(M_PI) / 180.0f;
            z *= static_cast<float>(M_PI) / 180.0f;
            float cx = cosf(x);
            float sx = sinf(x);
            float cy = cosf(y);
            float sy = sinf(y);
            float cz = cosf(z);
            float sz = sinf(z);
            float cxsy = cx * sy;
            float sxsy = sx * sy;

            m[M00] = cy * cz;
            m[M10] = -cy * sz;
            m[M20] = sy;
            m[M30] = 0;

            m[M01] = cxsy * cz + cx * sz;
            m[M11] = -cxsy * sy + cx * cz;
            m[M21] = -sx * cy;
            m[M31] = 0;

            m[M02] = -sxsy * cz + sx * sz;
            m[M12] = sxsy * sz + sx * cz;
            m[M22] = cx * cy;
            m[M32] = 0;

            m[M03] = 0;
            m[M13] = 0;
            m[M23] = 0;
            m[M33] = 1;

            return m;
        }

		static Matrix &translate(Matrix &m, float x, float y, float z) {
			for (int i = 0; i < 4; ++i) {
				m[12 + i] += m[i] * x + m[i + 4] * y + m[i + 8] * z;
			}
			return m;
		}

		static inline float matrix4_det(float *m) {
			return m[M30] * m[M21] * m[M12] * m[M03] - m[M20] * m[M31] * m[M12] * m[M03] -
				   m[M30] * m[M11] * m[M22] * m[M03] + m[M10] * m[M31] * m[M22] * m[M03] +
				   m[M20] * m[M11] * m[M32] * m[M03] - m[M10] * m[M21] * m[M32] * m[M03] -
				   m[M30] * m[M21] * m[M02] * m[M13] + m[M20] * m[M31] * m[M02] * m[M13] +
				   m[M30] * m[M01] * m[M22] * m[M13] - m[M00] * m[M31] * m[M22] * m[M13] -
				   m[M20] * m[M01] * m[M32] * m[M13] + m[M00] * m[M21] * m[M32] * m[M13] +
				   m[M30] * m[M11] * m[M02] * m[M23] - m[M10] * m[M31] * m[M02] * m[M23] -
				   m[M30] * m[M01] * m[M12] * m[M23] + m[M00] * m[M31] * m[M12] * m[M23] +
				   m[M10] * m[M01] * m[M32] * m[M23] - m[M00] * m[M11] * m[M32] * m[M23] -
				   m[M20] * m[M11] * m[M02] * m[M33] + m[M10] * m[M21] * m[M02] * m[M33] +
				   m[M20] * m[M01] * m[M12] * m[M33] - m[M00] * m[M21] * m[M12] * m[M33] -
				   m[M10] * m[M01] * m[M22] * m[M33] + m[M00] * m[M11] * m[M22] * m[M33];
		}

		static bool invert(const Matrix &m, Matrix &out) {
			float d = determinant(m);
			if (d == 0)
				return false;

			float inv = 1.0f / d;

			out[M00] =
					m[M12] * m[M23] * m[M31] - m[M13] * m[M22] * m[M31] + m[M13] * m[M21] * m[M32] -
					m[M11] * m[M23] * m[M32] - m[M12] * m[M21] * m[M33] +
					m[M11] * m[M22] * m[M33] * inv;

			out[M01] =
					m[M03] * m[M22] * m[M31] - m[M02] * m[M23] * m[M31] - m[M03] * m[M21] * m[M32] +
					m[M01] * m[M23] * m[M32] + m[M02] * m[M21] * m[M33] -
					m[M01] * m[M22] * m[M33] * inv;

			out[M02] =
					m[M02] * m[M13] * m[M31] - m[M03] * m[M12] * m[M31] + m[M03] * m[M11] * m[M32] -
					m[M01] * m[M13] * m[M32] - m[M02] * m[M11] * m[M33] +
					m[M01] * m[M12] * m[M33] * inv;

			out[M03] =
					m[M03] * m[M12] * m[M21] - m[M02] * m[M13] * m[M21] - m[M03] * m[M11] * m[M22] +
					m[M01] * m[M13] * m[M22] + m[M02] * m[M11] * m[M23] -
					m[M01] * m[M12] * m[M23] * inv;

			out[M10] =
					m[M13] * m[M22] * m[M30] - m[M12] * m[M23] * m[M30] - m[M13] * m[M20] * m[M32] +
					m[M10] * m[M23] * m[M32] + m[M12] * m[M20] * m[M33] -
					m[M10] * m[M22] * m[M33] * inv;

			out[M11] =
					m[M02] * m[M23] * m[M30] - m[M03] * m[M22] * m[M30] + m[M03] * m[M20] * m[M32] -
					m[M00] * m[M23] * m[M32] - m[M02] * m[M20] * m[M33] +
					m[M00] * m[M22] * m[M33] * inv;

			out[M12] =
					m[M03] * m[M12] * m[M30] - m[M02] * m[M13] * m[M30] - m[M03] * m[M10] * m[M32] +
					m[M00] * m[M13] * m[M32] + m[M02] * m[M10] * m[M33] -
					m[M00] * m[M12] * m[M33] * inv;

			out[M13] =
					m[M02] * m[M13] * m[M20] - m[M03] * m[M12] * m[M20] + m[M03] * m[M10] * m[M22] -
					m[M00] * m[M13] * m[M22] - m[M02] * m[M10] * m[M23] +
					m[M00] * m[M12] * m[M23] * inv;
			out[M20] =
					m[M11] * m[M23] * m[M30] - m[M13] * m[M21] * m[M30] + m[M13] * m[M20] * m[M31] -
					m[M10] * m[M23] * m[M31] - m[M11] * m[M20] * m[M33] +
					m[M10] * m[M21] * m[M33] * inv;

			out[M21] =
					m[M03] * m[M21] * m[M30] - m[M01] * m[M23] * m[M30] - m[M03] * m[M20] * m[M31] +
					m[M00] * m[M23] * m[M31] + m[M01] * m[M20] * m[M33] -
					m[M00] * m[M21] * m[M33] * inv;

			out[M22] =
					m[M01] * m[M13] * m[M30] - m[M03] * m[M11] * m[M30] + m[M03] * m[M10] * m[M31] -
					m[M00] * m[M13] * m[M31] - m[M01] * m[M10] * m[M33] +
					m[M00] * m[M11] * m[M33] * inv;

			out[M23] =
					m[M03] * m[M11] * m[M20] - m[M01] * m[M13] * m[M20] - m[M03] * m[M10] * m[M21] +
					m[M00] * m[M13] * m[M21] + m[M01] * m[M10] * m[M23] -
					m[M00] * m[M11] * m[M23] * inv;

			out[M30] =
					m[M12] * m[M21] * m[M30] - m[M11] * m[M22] * m[M30] - m[M12] * m[M20] * m[M31] +
					m[M10] * m[M22] * m[M31] + m[M11] * m[M20] * m[M32] -
					m[M10] * m[M21] * m[M32] * inv;

			out[M31] =
					m[M01] * m[M22] * m[M30] - m[M02] * m[M21] * m[M30] + m[M02] * m[M20] * m[M31] -
					m[M00] * m[M22] * m[M31] - m[M01] * m[M20] * m[M32] +
					m[M00] * m[M21] * m[M32] * inv;

			out[M32] =
					m[M02] * m[M11] * m[M30] - m[M01] * m[M12] * m[M30] - m[M02] * m[M10] * m[M31] +
					m[M00] * m[M12] * m[M31] + m[M01] * m[M10] * m[M32] -
					m[M00] * m[M11] * m[M32] * inv;

			out[M33] =
					m[M01] * m[M12] * m[M20] - m[M02] * m[M11] * m[M20] + m[M02] * m[M10] * m[M21] -
					m[M00] * m[M12] * m[M21] - m[M01] * m[M10] * m[M22] +
					m[M00] * m[M11] * m[M22] * inv;

			return true;
		}

		static Vector3 transform(const Matrix &m, const Vector3 &point) {
			float x = (point.x() * m[M00] + point.y() * m[M01] + point.z() * m[M02] + m[M03]);
			float y = (point.x() * m[M10] + point.y() * m[M11] + point.z() * m[M12] + m[M13]);
			float z = (point.x() * m[M20] + point.y() * m[M21] + point.z() * m[M22] + m[M23]);
			return {x, y, z};
		}

		static Vector3 project(const Matrix &m, const Vector3 &point) {
			float inv_w = 1.0f / (point.x() * m[M30] + point.y() * m[M31] + point.z() * m[M32] + m[M33]);
            return transform(m, point) * inv_w;
		}
	};
}
