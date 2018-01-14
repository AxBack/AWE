#include "../pch.h"

namespace Electric {
    struct Vertex
    {
        GLfloat x, y, z;
    };

    struct ParticleInstance
    {
        GLfloat x,y,z,size,r,g,b,a;
    };

	struct NodeInstance
	{
		GLfloat x,y,z,charge;
	};
};
