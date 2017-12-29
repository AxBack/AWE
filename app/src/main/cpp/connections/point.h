#pragma once

#include "../engine/vector3.h"

namespace Connections {

	struct Point {
		Math::Vector3 position;
		Math::Vector3 color;
		float size;
		float time;
		Math::Vector3 force;
	};

	struct Connection {
		UINT i1, i2;
		float minDistance;
		float maxDistance;
		float minDistanceSq;
		float maxDistanceSq;
		float tension;
	};
}