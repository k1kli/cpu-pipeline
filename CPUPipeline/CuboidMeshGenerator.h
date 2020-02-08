#pragma once
#include "VirtualMeshGenerator.h"
class CuboidMeshGenerator : public VirtualMeshGenerator
{
public:
	CuboidMeshGenerator(float width, float height, float depth)
		:width(width),
		height(height),
		depth(depth) {}
private:
	float width;
	float height;
	float depth;

	void validateParameters();
	void resizeVectors();
	void setValues();
	void setTriangles();
};

