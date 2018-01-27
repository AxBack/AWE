#pragma once

#include "../pch.h"

namespace Electric {
    struct PositionVertex
    {
        GLfloat x, y, z;
    };

	struct TexturedVertex
	{
		GLfloat x,y,z,u,v;
	};

	struct DischargeVertex
	{
		GLfloat x, y, z, distortInfluence, dt;
	};

    struct ParticleInstance
    {
        GLfloat x,y,z,size,r,g,b,a;
    };

	struct NodeInstance
	{
		GLfloat x,y,z,size,charge,r,g,b;
	};

	struct DischargeInstance
	{
		GLfloat sx,sy,sz, dx,dy,dz, rand;
	};
};
