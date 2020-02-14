#include "CuboidMeshGenerator.h"

CuboidMeshGenerator::CuboidMeshGenerator(float width, float height, float depth)
	:VirtualMeshGenerator({ "width", "height", "depth" }, "cuboid")
{
	parameters = { width, height, depth };
}
std::shared_ptr<VirtualMeshGenerator> CuboidMeshGenerator::getInstance() const
{
	return std::shared_ptr<VirtualMeshGenerator>(new CuboidMeshGenerator(*this));
}

void CuboidMeshGenerator::validateParameters()
{
	width = parameters[0];
	height = parameters[1];
	depth = parameters[2];
	if (width <= 0.0f || height <= 0.0f || depth <= 0.0f)
		throw "dimensions should be positive";
}

void CuboidMeshGenerator::resizeVectors()
{
	//vectors set directly, no need for resizing
}

void CuboidMeshGenerator::setValues()
{
	vertices = {
		   {0.0f,0.0f,0.0f}, {0.0f,height,0.0f}, {width,height,0.0f}, {width,0.0f,0.0f},
		   {0.0f,0.0f,depth}, {0.0f,height,depth}, {width,height,depth}, {width,0.0f,depth}
		};
	//front, back, right, left, up, down
	normals = {
		{0.0f,0.0f,1.0f},{0.0f,0.0f,-1.0f},
		{1.0f,0.0f,0.0f},{-1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},{0.0f,-1.0f,0.0f},
		};
	tangents = {
		{1.0f,0.0f,0.0f},{-1.0f,0.0f,0.0f},
		{0.0f,0.0f,-1.0f},{0.0f,0.0f,1.0f},
		{1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f}
		};

	uv = {
		{0,0},//bottom left
		{0,1},//top left
		{1,0},//bottom right
		{1,1}//top right
		};
}

void CuboidMeshGenerator::setTriangles()
{
	//back:
	//1 2
	//0 3

	//front:
	//5 6
	//4 7
	triangles = {
			{4,7,6},{4,6,5},//front
			{3,0,1},{3,1,2},//back
			{7,3,2},{7,2,6},//right
			{0,4,5},{0,5,1},//left
			{5,6,2},{5,2,1},//up
			{0,7,4},{0,3,7}//down
		};
	trianglesNormals = {
			{0,0,0},{0,0,0},//front
			{1,1,1},{1,1,1},//back
			{2,2,2},{2,2,2},//right
			{3,3,3},{3,3,3},//left
			{4,4,4},{4,4,4},//up
			{5,5,5},{5,5,5}//down
		};
	trianglesUV = {
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,3,1},{0,2,3}
		};
}
