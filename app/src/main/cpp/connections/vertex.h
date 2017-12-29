#pragma once

namespace Connections {

	struct Vertex {
		GLfloat x, y, z, tag;
	};

	struct PointInstance {
		GLfloat x, y, z, size;
		GLfloat r, g, b;
	};

	struct ConnectionInstance {
		GLfloat startx, starty, startz, endx, endy, endz, tension;
		GLfloat startr, startg, startb, endr, endg, endb;
	};
}