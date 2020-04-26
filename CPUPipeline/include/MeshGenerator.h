#pragma once
#include "Mesh.h"
class MeshGenerator
{
public:
	Mesh getCuboidMesh(float width, float height, float depth);
	Mesh getCylinderMesh(float H, float R, int netDivisions);
	Mesh getSphereMesh(float R, int netVerticalLinesCount, int netHorizontalLinesCount);
	Mesh getConeMesh(float H, float R, int netDivisions);
};

