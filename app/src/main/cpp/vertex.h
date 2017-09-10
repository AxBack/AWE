#pragma once

struct Vertex
{
	GLfloat x,y,z;
};

struct PointInstance
{
	GLfloat x,y,z, size;
};

struct ConnectionInstance
{
	GLfloat startx,starty,startz, endx,endy,endz, tension;
};