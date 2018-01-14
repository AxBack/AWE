#include "../pch.h"

namespace Electric {
    struct PositionVertex
    {
        GLfloat x, y, z;
    };

	struct ChargeVertex
	{
		GLfloat x, y, z, dt;
	};

    struct ParticleInstance
    {
        GLfloat x,y,z,size,r,g,b,a;
    };

	struct NodeInstance
	{
		GLfloat x,y,z,charge;
	};

	struct ChargeInstance
	{
		GLfloat sx,sy,sz, dx,dy,dz;
	};
};
