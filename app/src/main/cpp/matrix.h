#pragma once
//
// Created by AxB on 4/23/2017.
//

#include <string.h>
#include "vector3.h"

class Matrix
{
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

	float mData[16];

public:

	Matrix()
	{
	}

	const float* const data() const
	{ return mData; }

	Matrix operator*(const Matrix& rhs)
	{
		return multiply(*this, rhs);
	}

	float operator[](const int index) const
	{
		return mData[index];
	}

	Vector3 transform(const Vector3& point)
	{
		return Matrix::transform(*this, point);
	}

	Vector3 project(const Vector3& point)
	{
		return Matrix::project(*this, point);
	}

	static void identity(Matrix& matrix)
	{
		memset(matrix.mData, 0, sizeof(matrix.mData));
		matrix.mData[M00] = matrix.mData[M11] = matrix.mData[M22] = matrix.mData[M33] = 1.0f;
	}

	static Matrix multiply(const Matrix& left, const Matrix& right)
	{
		Matrix ret;

		const float* pL = left.mData;
		const float* pR = right.mData;

		ret.mData[M00] =
				pR[M00] * pL[M00] + pR[M10] * pL[M01] + pR[M20] * pL[M02] + pR[M30] * pL[M03];
		ret.mData[M10] =
				pR[M00] * pL[M10] + pR[M10] * pL[M11] + pR[M20] * pL[M12] + pR[M30] * pL[M13];
		ret.mData[M20] =
				pR[M00] * pL[M20] + pR[M10] * pL[M21] + pR[M20] * pL[M22] + pR[M30] * pL[M23];
		ret.mData[M30] =
				pR[M00] * pL[M30] + pR[M10] * pL[M31] + pR[M20] * pL[M32] + pR[M30] * pL[M33];

		ret.mData[M01] =
				pR[M01] * pL[M00] + pR[M11] * pL[M01] + pR[M21] * pL[M02] + pR[M31] * pL[M03];
		ret.mData[M11] =
				pR[M01] * pL[M10] + pR[M11] * pL[M11] + pR[M21] * pL[M12] + pR[M31] * pL[M13];
		ret.mData[M21] =
				pR[M01] * pL[M20] + pR[M11] * pL[M21] + pR[M21] * pL[M22] + pR[M31] * pL[M23];
		ret.mData[M31] =
				pR[M01] * pL[M30] + pR[M11] * pL[M31] + pR[M21] * pL[M32] + pR[M31] * pL[M33];

		ret.mData[M02] =
				pR[M02] * pL[M00] + pR[M12] * pL[M01] + pR[M22] * pL[M02] + pR[M32] * pL[M03];
		ret.mData[M12] =
				pR[M02] * pL[M10] + pR[M12] * pL[M11] + pR[M22] * pL[M12] + pR[M32] * pL[M13];
		ret.mData[M22] =
				pR[M02] * pL[M20] + pR[M12] * pL[M21] + pR[M22] * pL[M22] + pR[M32] * pL[M23];
		ret.mData[M32] =
				pR[M02] * pL[M30] + pR[M12] * pL[M31] + pR[M22] * pL[M32] + pR[M32] * pL[M33];

		ret.mData[M03] =
				pR[M03] * pL[M00] + pR[M13] * pL[M01] + pR[M23] * pL[M02] + pR[M33] * pL[M03];
		ret.mData[M13] =
				pR[M03] * pL[M10] + pR[M13] * pL[M11] + pR[M23] * pL[M12] + pR[M33] * pL[M13];
		ret.mData[M23] =
				pR[M03] * pL[M20] + pR[M13] * pL[M21] + pR[M23] * pL[M22] + pR[M33] * pL[M23];
		ret.mData[M33] =
				pR[M03] * pL[M30] + pR[M13] * pL[M31] + pR[M23] * pL[M32] + pR[M33] * pL[M33];

		return ret;
	}

	static Matrix& frustum(Matrix& m, float left, float right, float bottom, float top, float near,
						   float far)
	{
		float width = 1.0f / (right - left);
		float height = 1.0f / (top - bottom);
		float depth = 1.0f / (near - far);
		float x = 2.0f * (near * width);
		float y = 2.0f * (near * height);
		float a = (right + left) * width;
		float b = (top + bottom) * height;
		float c = (far + near) * depth;
		float d = 2.0f * (far * near * depth);

		m.mData[M00] = x;
		m.mData[M11] = y;
		m.mData[M02] = a;
		m.mData[M12] = b;
		m.mData[M22] = c;
		m.mData[M23] = d;
		m.mData[M32] = -1.0f;
		m.mData[M10] = 0.0f;
		m.mData[M20] = 0.0f;
		m.mData[M30] = 0.0f;
		m.mData[M01] = 0.0f;
		m.mData[M21] = 0.0f;
		m.mData[M31] = 0.0f;
		m.mData[M03] = 0.0f;
		m.mData[M13] = 0.0f;
		m.mData[M33] = 0.0f;

		return m;
	}

	static Matrix& ortho(Matrix& m, float left, float right, float bottom, float top, float near,
						 float far)
	{
		m.mData[M00] = 2.0f / (right - left);
		m.mData[M10] = 0.0f;
		m.mData[M20] = 0.0f;
		m.mData[M30] = 0.0f;
		m.mData[M01] = 0.0f;
		m.mData[M11] = 2.0f / (top - bottom);
		m.mData[M21] = 0.0f;
		m.mData[M31] = 0.0f;
		m.mData[M02] = 0.0f;
		m.mData[M12] = 0.0f;
		m.mData[M22] = 2.0f / (far - near);
		m.mData[M32] = 0.0f;
		m.mData[M03] = -(right + left) / (right - left);
		m.mData[M13] = -(top + bottom) / (top - bottom);
		m.mData[M23] = -(near + far) / (far - near);
		m.mData[M33] = 1.0f;

		return m;
	}

	static Matrix& lookAt(Matrix& m, const Vector3& eye, const Vector3& at, const Vector3& up)
	{
		Vector3 f = at - eye;
		f.normalize();

		Vector3 s = f.cross(up);
		s.normalize();

		Vector3 u = s.cross(f);

		m.mData[M00] = s.x;
		m.mData[M10] = u.x;
		m.mData[M20] = -f.x;
		m.mData[M30] = 0.0f;
		m.mData[M01] = s.y;
		m.mData[M11] = u.y;
		m.mData[M21] = -f.y;
		m.mData[M31] = 0.0f;
		m.mData[M02] = s.z;
		m.mData[M12] = u.z;
		m.mData[M22] = -f.z;
		m.mData[M32] = 0.0f;
		m.mData[M03] = 0.0f;
		m.mData[M13] = 0.0f;
		m.mData[M23] = 0.0f;
		m.mData[M33] = 1.0f;

		return translate(m, -eye.x, -eye.y, -eye.z);
	}

	static float determinant(const Matrix& m)
	{
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

	static Matrix& translate(Matrix& m, float x, float y, float z)
	{
		for(int i = 0; i < 4; ++i)
		{
			m.mData[12 + i] += m.mData[i] * x + m.mData[i + 4] * y + m.mData[i + 8] * z;
		}
		return m;
	}

	static inline float matrix4_det(float* m)
	{
		return m[M30] * m[M21] * m[M12] * m[M03] - m[M20] * m[M31] * m[M12] * m[M03] -
			   m[M30] * m[M11]
			   * m[M22] * m[M03] + m[M10] * m[M31] * m[M22] * m[M03] +
			   m[M20] * m[M11] * m[M32] * m[M03] - m[M10]
												   * m[M21] * m[M32] * m[M03] -
			   m[M30] * m[M21] * m[M02] * m[M13] + m[M20] * m[M31] * m[M02] * m[M13]
			   + m[M30] * m[M01] * m[M22] * m[M13] - m[M00] * m[M31] * m[M22] * m[M13] -
			   m[M20] * m[M01] * m[M32]
			   * m[M13] + m[M00] * m[M21] * m[M32] * m[M13] + m[M30] * m[M11] * m[M02] * m[M23] -
			   m[M10] * m[M31]
			   * m[M02] * m[M23] - m[M30] * m[M01] * m[M12] * m[M23] +
			   m[M00] * m[M31] * m[M12] * m[M23] + m[M10]
												   * m[M01] * m[M32] * m[M23] -
			   m[M00] * m[M11] * m[M32] * m[M23] - m[M20] * m[M11] * m[M02] * m[M33]
			   + m[M10] * m[M21] * m[M02] * m[M33] + m[M20] * m[M01] * m[M12] * m[M33] -
			   m[M00] * m[M21] * m[M12]
			   * m[M33] - m[M10] * m[M01] * m[M22] * m[M33] + m[M00] * m[M11] * m[M22] * m[M33];
	}

	static bool invert(const Matrix& m, Matrix& out)
	{
		float d = determinant(m);
		if(d == 0)
			return false;

		float inv = 1.0f / d;

		out[M00] = m[M12] * m[M23] * m[M31] - m[M13] * m[M22] * m[M31] + m[M13] * m[M21] * m[M32] - m[M11] * m[M23] * m[M32] - m[M12] * m[M21] * m[M33] + m[M11] * m[M22] * m[M33] * inv;
		out[M01] = m[M03] * m[M22] * m[M31] - m[M02] * m[M23] * m[M31] - m[M03] * m[M21] * m[M32] + m[M01] * m[M23] * m[M32] + m[M02] * m[M21] * m[M33] - m[M01] * m[M22] * m[M33] * inv;
		out[M02] = m[M02] * m[M13] * m[M31] - m[M03] * m[M12] * m[M31] + m[M03] * m[M11] * m[M32] - m[M01] * m[M13] * m[M32] - m[M02] * m[M11] * m[M33] + m[M01] * m[M12] * m[M33] * inv;
		out[M03] = m[M03] * m[M12] * m[M21] - m[M02] * m[M13] * m[M21] - m[M03] * m[M11] * m[M22] + m[M01] * m[M13] * m[M22] + m[M02] * m[M11] * m[M23] - m[M01] * m[M12] * m[M23] * inv;
		out[M10] = m[M13] * m[M22] * m[M30] - m[M12] * m[M23] * m[M30] - m[M13] * m[M20] * m[M32] + m[M10] * m[M23] * m[M32] + m[M12] * m[M20] * m[M33] - m[M10] * m[M22] * m[M33] * inv;
		out[M11] = m[M02] * m[M23] * m[M30] - m[M03] * m[M22] * m[M30] + m[M03] * m[M20] * m[M32] - m[M00] * m[M23] * m[M32] - m[M02] * m[M20] * m[M33] + m[M00] * m[M22] * m[M33] * inv;
		out[M12] = m[M03] * m[M12] * m[M30] - m[M02] * m[M13] * m[M30] - m[M03] * m[M10] * m[M32] + m[M00] * m[M13] * m[M32] + m[M02] * m[M10] * m[M33] - m[M00] * m[M12] * m[M33] * inv;
		out[M13] = m[M02] * m[M13] * m[M20] - m[M03] * m[M12] * m[M20] + m[M03] * m[M10] * m[M22] - m[M00] * m[M13] * m[M22] - m[M02] * m[M10] * m[M23] + m[M00] * m[M12] * m[M23] * inv;
		out[M20] = m[M11] * m[M23] * m[M30] - m[M13] * m[M21] * m[M30] + m[M13] * m[M20] * m[M31] - m[M10] * m[M23] * m[M31] - m[M11] * m[M20] * m[M33] + m[M10] * m[M21] * m[M33] * inv;
		out[M21] = m[M03] * m[M21] * m[M30] - m[M01] * m[M23] * m[M30] - m[M03] * m[M20] * m[M31] + m[M00] * m[M23] * m[M31] + m[M01] * m[M20] * m[M33] - m[M00] * m[M21] * m[M33] * inv;
		out[M22] = m[M01] * m[M13] * m[M30] - m[M03] * m[M11] * m[M30] + m[M03] * m[M10] * m[M31] - m[M00] * m[M13] * m[M31] - m[M01] * m[M10] * m[M33] + m[M00] * m[M11] * m[M33] * inv;
		out[M23] = m[M03] * m[M11] * m[M20] - m[M01] * m[M13] * m[M20] - m[M03] * m[M10] * m[M21] + m[M00] * m[M13] * m[M21] + m[M01] * m[M10] * m[M23] - m[M00] * m[M11] * m[M23] * inv;
		out[M30] = m[M12] * m[M21] * m[M30] - m[M11] * m[M22] * m[M30] - m[M12] * m[M20] * m[M31] + m[M10] * m[M22] * m[M31] + m[M11] * m[M20] * m[M32] - m[M10] * m[M21] * m[M32] * inv;
		out[M31] = m[M01] * m[M22] * m[M30] - m[M02] * m[M21] * m[M30] + m[M02] * m[M20] * m[M31] - m[M00] * m[M22] * m[M31] - m[M01] * m[M20] * m[M32] + m[M00] * m[M21] * m[M32] * inv;
		out[M32] = m[M02] * m[M11] * m[M30] - m[M01] * m[M12] * m[M30] - m[M02] * m[M10] * m[M31] + m[M00] * m[M12] * m[M31] + m[M01] * m[M10] * m[M32] - m[M00] * m[M11] * m[M32] * inv;
		out[M33] = m[M01] * m[M12] * m[M20] - m[M02] * m[M11] * m[M20] + m[M02] * m[M10] * m[M21] - m[M00] * m[M12] * m[M21] - m[M01] * m[M10] * m[M22] + m[M00] * m[M11] * m[M22] * inv;

		return true;
	}

	static Vector3 transform(const Matrix& m, const Vector3& point)
	{
		float x = (point.x * m[M00] + point.y * m[M01] + point.z * m[M02] + m[M03]);
		float y = (point.x * m[M10] + point.y * m[M11] + point.z * m[M12] + m[M13]);
		float z = (point.x * m[M20] + point.y * m[M21] + point.z * m[M22] + m[M23]);
		return {x, y, z};
	}

	static Vector3 project(const Matrix& m, const Vector3& point)
	{
		float inv_w = 1.0f / (point.x * m[M30] + point.y * m[M31] + point.z * m[M32] + m[M33]);
		float x = (point.x * m[M00] + point.y * m[M01] + point.z * m[M02] + m[M03]) * inv_w;
		float y = (point.x * m[M10] + point.y * m[M11] + point.z * m[M12] + m[M13]) * inv_w;
		float z = (point.x * m[M20] + point.y * m[M21] + point.z * m[M22] + m[M23]) * inv_w;
		return {x, y, z};
	}
};
