#include "MeshGenerator.h"
#include "TransformationMatrices.h"
#include "SphereMeshGenerator.h"
#include "CuboidMeshGenerator.h"
#include "CyllinderMeshGenerator.h"
#include "ConeMeshGenerator.h"
Mesh MeshGenerator::getCuboidMesh(float width, float height, float depth)
{
	CuboidMeshGenerator generator(width, height, depth);
	return generator.getMesh();
}

Mesh MeshGenerator::getCylinderMesh(float H, float R, int netDivisions)
{
	CyllinderMeshGenerator generator(H, R, netDivisions);
	return generator.getMesh();
}

Mesh MeshGenerator::getSphereMesh(float R, int netVerticalLinesCount, int netHorizontalLinesCount)
{
	SphereMeshGenerator generator(R, netVerticalLinesCount, netHorizontalLinesCount);
	return generator.getMesh();
}

Mesh MeshGenerator::getConeMesh(float H, float R, int netDivisions)
{
	ConeMeshGenerator generator(H, R, netDivisions);
	return generator.getMesh();
}
