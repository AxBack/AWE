#pragma once

#include "vector3.h"

struct Point
{
	Vector3 position;
	Vector3 color;
	float size;
	float time;
	Vector3 force;
};

struct Connection
{
	UINT i1, i2;
	float minDistance;
	float maxDistance;
	float minDistanceSq;
	float maxDistanceSq;
	float tension;
};