#pragma once
#include "Mesh.h"
class MeshGenerator
{
public:
	Mesh getCubeMesh();
	Mesh getCylinderMesh(float H, float R, int netDivisions);
	Mesh getSphereMesh(float R, int netVerticalLinesCount, int netHorizontalLinesCount);
	Mesh getConeMesh(float H, float R, int netDivisions);
};

