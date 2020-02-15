#pragma once
#include "VirtualMeshGenerator.h"
class CuboidMeshGenerator : public VirtualMeshGenerator
{
public:
	CuboidMeshGenerator(float width, float height, float depth);
	CuboidMeshGenerator();
	std::shared_ptr<VirtualMeshGenerator> getInstance() const;
	MeshType getType() const { return MeshType::CUBE; }
private:
	float width;
	float height;
	float depth;

	void validateParameters();
	void resizeVectors();
	void setValues();
	void setTriangles();
};

