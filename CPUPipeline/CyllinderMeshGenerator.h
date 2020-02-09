#pragma once
#include "VirtualMeshGenerator.h"
class CyllinderMeshGenerator : public VirtualMeshGenerator
{
public:
	CyllinderMeshGenerator(float height, float radius, int netDivisions)
		:height(height),
		radius(radius),
		netDivisions(netDivisions) {}
private:
	const int BOTTOM = 0;
	const int TOP = 1;
	float height;
	float radius;
	int netDivisions;

	void validateParameters();
	void resizeVectors();
	void setValues();
	void setTriangles();

	int getCenterVertexIndex(int topOrBottom);
	int getCenterNormTangIndex(int topOrBottom);
	int getWallVertexIndex(int topOrBottom, int wallId);
	int getWallNormTangIndex(int wallId);
	int getWallUVIndex(int topOrBottom, int wallId);
	int getCircleEdgeUVIndex(int topOrBottom, int i);
	int getCenterUVIndex(int topOrBottom);

	int getWallTriangleIndex(int wallId, int triangleInPair);
	int getCircleTriangleIndex(int topOrBottom, int onCircleId);
};

