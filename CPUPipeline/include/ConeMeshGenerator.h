#pragma once
#include "VirtualMeshGenerator.h"
#include "MeshType.h"
class ConeMeshGenerator : public VirtualMeshGenerator
{
public:
	ConeMeshGenerator(float height, float radius, int netDivisions);
	ConeMeshGenerator();
private:
	const int BOTTOM = 0;
	const int MIDDLE = 1;
	const int TOP = 2;
	float height = -1.0f;
	float radius = -1.0f;
	int netDivisions = -1;


	std::shared_ptr<VirtualMeshGenerator> getInstance() const;
	MeshType getType() const { return MeshType::CONE; }
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

