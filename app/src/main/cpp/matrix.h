#pragma once
//
// Created by AxB on 4/23/2017.
//

#include <string.h>
#include "vector3.h"

class Matrix
{
private:
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

	static void identity(Matrix& matrix)
	{
		memset(matrix.mData, 0, sizeof(matrix.mData));
		matrix.mData[0] = matrix.mData[4] = matrix.mData[10] = matrix.mData[15] = 1.0f;
	}

	static Matrix multiply(const Matrix& left, const Matrix& right)
	{
		Matrix ret;

		const float* pL = left.mData;
		const float* pR = right.mData;

		ret.mData[0] = pR[0] * pL[0] + pR[1] * pL[4] + pR[2] * pL[8] + pR[3] * pL[12];
		ret.mData[1] = pR[0] * pL[1] + pR[1] * pL[5] + pR[2] * pL[9] + pR[3] * pL[13];
		ret.mData[2] = pR[0] * pL[2] + pR[1] * pL[6] + pR[2] * pL[10] + pR[3] * pL[14];
		ret.mData[3] = pR[0] * pL[3] + pR[1] * pL[7] + pR[2] * pL[11] + pR[3] * pL[15];

		ret.mData[4] = pR[4] * pL[0] + pR[5] * pL[4] + pR[6] * pL[8] + pR[7] * pL[12];
		ret.mData[5] = pR[4] * pL[1] + pR[5] * pL[5] + pR[6] * pL[9] + pR[7] * pL[13];
		ret.mData[6] = pR[4] * pL[2] + pR[5] * pL[6] + pR[6] * pL[10] + pR[7] * pL[14];
		ret.mData[7] = pR[4] * pL[3] + pR[5] * pL[7] + pR[6] * pL[11] + pR[7] * pL[15];

		ret.mData[8] = pR[8] * pL[0] + pR[9] * pL[4] + pR[10] * pL[8] + pR[11] * pL[12];
		ret.mData[9] = pR[8] * pL[1] + pR[9] * pL[5] + pR[10] * pL[9] + pR[11] * pL[13];
		ret.mData[10] = pR[8] * pL[2] + pR[9] * pL[6] + pR[10] * pL[10] + pR[11] * pL[14];
		ret.mData[11] = pR[8] * pL[3] + pR[9] * pL[7] + pR[10] * pL[11] + pR[11] * pL[15];

		ret.mData[12] = pR[12] * pL[0] + pR[13] * pL[4] + pR[14] * pL[8] + pR[15] * pL[12];
		ret.mData[13] = pR[12] * pL[1] + pR[13] * pL[5] + pR[14] * pL[9] + pR[15] * pL[13];
		ret.mData[14] = pR[12] * pL[2] + pR[13] * pL[6] + pR[14] * pL[10] + pR[15] * pL[14];
		ret.mData[15] = pR[12] * pL[3] + pR[13] * pL[7] + pR[14] * pL[11] + pR[15] * pL[15];

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

		m.mData[0] = x;
		m.mData[5] = y;
		m.mData[8] = a;
		m.mData[9] = b;
		m.mData[10] = c;
		m.mData[14] = d;
		m.mData[11] = -1.0f;
		m.mData[1] = 0.0f;
		m.mData[2] = 0.0f;
		m.mData[3] = 0.0f;
		m.mData[4] = 0.0f;
		m.mData[6] = 0.0f;
		m.mData[7] = 0.0f;
		m.mData[12] = 0.0f;
		m.mData[13] = 0.0f;
		m.mData[15] = 0.0f;

		return m;
	}

	static Matrix& ortho(Matrix& m, float left, float right, float bottom, float top, float near,
						 float far)
	{
		m.mData[0] = 2.0f / (right-left);
		m.mData[1] = 0.0f;
		m.mData[2] = 0.0f;
		m.mData[3] = 0.0f;
		m.mData[4] = 0.0f;
		m.mData[5] = 2.0f/(top-bottom);
		m.mData[6] = 0.0f;
		m.mData[7] = 0.0f;
		m.mData[8] = 0.0f;
		m.mData[9] = 0.0f;
		m.mData[10] = 2.0f/(far-near);
		m.mData[11] = 0.0f;
		m.mData[12] = -(right+left)/(right-left);
		m.mData[13] = -(top+bottom)/(top-bottom);
		m.mData[14] = -(near+far)/(far-near);
		m.mData[15] = 1.0f;

		return m;
	}

	static Matrix& lookAt(Matrix& m, const Vector3& eye, const Vector3& at, const Vector3& up)
	{
		Vector3 f = at - eye;
		f.normalize();

		Vector3 s = f.cross(up);
		s.normalize();

		Vector3 u = s.cross(f);

		m.mData[0] = s.x;
		m.mData[1] = u.x;
		m.mData[2] = -f.x;
		m.mData[3] = 0.0f;
		m.mData[4] = s.y;
		m.mData[5] = u.y;
		m.mData[6] = -f.y;
		m.mData[7] = 0.0f;
		m.mData[8] = s.z;
		m.mData[9] = u.z;
		m.mData[10] = -f.z;
		m.mData[11] = 0.0f;
		m.mData[12] = 0.0f;
		m.mData[13] = 0.0f;
		m.mData[14] = 0.0f;
		m.mData[15] = 1.0f;

		return translate(m, -eye.x, -eye.y, -eye.z);
	}

	static Matrix& translate(Matrix& m, float x, float y, float z)
	{
		for(int i = 0; i < 4; ++i)
		{
			m.mData[12 + i] += m.mData[i] * x + m.mData[i + 4] * y + m.mData[i + 8] * z;
		}
		return m;
	}
};
