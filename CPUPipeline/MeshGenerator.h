#pragma once
#include "Mesh.h"
class MeshGenerator
{
public:
	Mesh getCubeMesh();
	Mesh getCylinderMesh(int netDivisions);
	Mesh getSphereMesh(float R, int netVerticalLinesCount, int netHorizontalLinesCount);
};

