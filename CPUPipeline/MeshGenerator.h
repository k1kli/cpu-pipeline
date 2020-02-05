#pragma once
#include "Mesh.h"
class MeshGenerator
{
public:
	Mesh getCubeMesh();
	Mesh getCylinderMesh(int netDivisions);
};

