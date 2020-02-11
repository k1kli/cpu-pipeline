#pragma once
#include "Mesh.h"
#include "VirtualMeshGenerator.h"
class SphereMeshGenerator : public VirtualMeshGenerator
{
public:

	SphereMeshGenerator(float R, int netVerticalLinesCount, int netHorizontalLinesCount);
	std::shared_ptr<VirtualMeshGenerator> getInstance() const;
private:
	const int SOUTH_POLE = 0;
	const int NORTH_POLE = 1;
	float R;
	int netVerticalLinesCount;
	int netHorizontalLinesCount;



	void validateParameters();
	void resizeVectors();
	void setValues();
	void setTriangles();

	int getNonPoleVertNormTangIndex(int horizontalLine, int verticalLine);
	int getPoleVertexIndex(int pole);
	int getPoleNormTangIndex(int pole, int touchingTriangleId);
	int getNonPoleTriangleIndex(int horizontalLine, int verticalLine, int triangleInPair);
	int getPoleTriangleIndex(int pole, int touchingTriangleId);
	int getNonPoleUVIndex(int horizontalLine, int verticalLine);
	int getPoleUVIndex(int pole, int touchingTriangleId);
		
};

