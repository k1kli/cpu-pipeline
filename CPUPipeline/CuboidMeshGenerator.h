#pragma once
#include "VirtualMeshGenerator.h"
class CuboidMeshGenerator : public VirtualMeshGenerator
{
public:
	CuboidMeshGenerator(float width, float height, float depth);
	std::shared_ptr<VirtualMeshGenerator> getInstance() const;
private:
	float width;
	float height;
	float depth;

	void validateParameters();
	void resizeVectors();
	void setValues();
	void setTriangles();
};

