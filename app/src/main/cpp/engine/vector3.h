#pragma once

#include <math.h>

struct Vector3
{
	float x, y, z;

	Vector3& operator=(const Vector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	Vector3 operator*(const float scale) const
	{
		return {x * scale, y * scale, z * scale};
	}

	void operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
	}

	Vector3 operator+(const Vector3& rhs) const
	{
		return {x + rhs.x, y + rhs.y, z + rhs.z};
	}

	void operator+=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}

	Vector3 operator-(const Vector3& rhs) const
	{
		return {x - rhs.x, y - rhs.y, z - rhs.z};
	}

	void operator-=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}

	Vector3 operator*(const Vector3& rhs) const
	{
		return {x * rhs.x, y * rhs.y, z * rhs.z};
	}

	void operator*=(const Vector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
	}

	Vector3 operator/(const Vector3& rhs) const
	{
		return {x / rhs.x, y / rhs.y, z / rhs.z};
	}

	void operator/=(const Vector3& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
	}

	float length() const
	{
		return static_cast<float>(sqrt(lengthSq()));
	}

	float lengthSq() const
	{
		return x * x + y * y + z * z;
	}

	void normalize()
	{
		float l = 1.f / length();
		x *= l;
		y *= l;
		z *= l;
	}

	Vector3 cross(const Vector3& other) const
	{
		return {
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
		};
	}
};
