#pragma once
#include "Mesh.h"
#include "VirtualMeshGenerator.h"
class SphereMeshGenerator : public VirtualMeshGenerator
{
public:

	SphereMeshGenerator(float R, int netVerticalLinesCount, int netHorizontalLinesCount);
	SphereMeshGenerator();
	std::shared_ptr<VirtualMeshGenerator> getInstance() const;
private:
	const int SOUTH_POLE = 0;
	const int NORTH_POLE = 1;
	float R = -1.0f;
	int netVerticalLinesCount = -1;
	int netHorizontalLinesCount = -1;



	void validateParameters();
	void resizeVectors();
	void setValues();
	void setTriangles();
	MeshType getType() const { return MeshType::SPHERE; }

	int getNonPoleVertNormTangIndex(int horizontalLine, int verticalLine);
	int getPoleVertexIndex(int pole);
	int getPoleNormTangIndex(int pole, int touchingTriangleId);
	int getNonPoleTriangleIndex(int horizontalLine, int verticalLine, int triangleInPair);
	int getPoleTriangleIndex(int pole, int touchingTriangleId);
	int getNonPoleUVIndex(int horizontalLine, int verticalLine);
	int getPoleUVIndex(int pole, int touchingTriangleId);
		
};

