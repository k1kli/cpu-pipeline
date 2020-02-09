#pragma once
#include "VirtualMeshGenerator.h"
class ConeMeshGenerator : public VirtualMeshGenerator
{
public:
	ConeMeshGenerator(float height, float radius, int netDivisions)
		:height(height),
		radius(radius),
		netDivisions(netDivisions) {}
private:
	const int BOTTOM = 0;
	const int MIDDLE = 1;
	const int TOP = 2;
	float height;
	float radius;
	int netDivisions;

	void validateParameters();
	void resizeVectors();
	void setValues();
	void setTriangles();
	int getCenterVertexIndex(int topOrBottom);
	int getWallVertexIndex(int middleOrBottom, int wallId);
	int getCircleNormTangIndex();
	int getNormTangIndex(int wallId);
	int getWallUVIndex(int topMiddleBottom, int wallId);
	int getCircleCenterUVIndex();
	int getCircleEdgeUVIndex(int triangleId);

	int getWallTriangleIndex(int aboveBottomOrMiddle, int wallId, int triangleInPair);
	int getCircleTriangleIndex(int triangleId);
};

