#include "CyllinderMeshGenerator.h"
#include <TransformationMatrices.h>

CyllinderMeshGenerator::CyllinderMeshGenerator(float height, float radius, int netDivisions)
	:VirtualMeshGenerator({ "height", "radius", "net divisions" }, "cyllinder")
{
	parameters = { height, radius, (float)netDivisions };
}
std::shared_ptr<VirtualMeshGenerator> CyllinderMeshGenerator::getInstance()
{
	return std::shared_ptr<VirtualMeshGenerator>(new CyllinderMeshGenerator(*this));
}

void CyllinderMeshGenerator::validateParameters()
{
	height = parameters[0];
	radius = parameters[1];
	netDivisions = parameters[2];
	if (netDivisions < 3) throw "net divisions should be 3 or higher";
	if (height <= 0) throw "height should be positive";
	if (radius <= 0) throw "radius should be positive";
}

void CyllinderMeshGenerator::resizeVectors()
{
	//top and bottom centers and two vertices on left of each side
	vertices.resize(2 + netDivisions * 2);

	//top and bottom and one for each vertical edge
	normals.resize(2 + netDivisions);
	tangents.resize(2 + netDivisions);

	//two for each vertical wall and netDivisions for top and bottom
	triangles.resize(2 * netDivisions + 2 * netDivisions);
	trianglesNormals.resize(2 * netDivisions + 2 * netDivisions);
	trianglesUV.resize(2 * netDivisions + 2 * netDivisions);

	//at the left edge of the texture u must be one so it can't be the same as the start,
	//for top and bottom circle: one for each edge vertex and one for center
	uv.resize(2 * (netDivisions + 1) + 2 * (netDivisions + 1));
}

void CyllinderMeshGenerator::setValues()
{
	vertices[getCenterVertexIndex(BOTTOM)] = glm::vec3(0.0f, -height * 0.5f, 0.0f);
	normals[getCenterNormTangIndex(BOTTOM)] = glm::vec3(0.0f, -1.0f, 0.0f);
	tangents[getCenterNormTangIndex(BOTTOM)] = glm::vec3(1.0f, 0.0f, 0.0f);

	vertices[getCenterVertexIndex(TOP)] = glm::vec3(0.0f, height * 0.5f, 0.0f);
	normals[getCenterNormTangIndex(TOP)] = glm::vec3(0.0f, 1.0f, 0.0f);
	tangents[getCenterNormTangIndex(TOP)] = glm::vec3(-1.0f, 0.0f, 0.0f);
	
	uv[getCenterUVIndex(BOTTOM)] = glm::vec2(0.5f, 0.5f);
	uv[getCenterUVIndex(TOP)] = glm::vec2(0.5f, 0.5f);


	float d = 2.0f * 3.1415f / netDivisions;
	glm::mat4 normalToTangentRotation =
		TransformationMatrices::getRotationMatrix(0.5f * 3.1415f, glm::vec3({ 0.0f,1.0f,0.0f }));
	for (int i = 0; i < netDivisions; i++)
	{
		vertices[getWallVertexIndex(BOTTOM, i)] = glm::vec3({
			glm::cos(d * i) * radius, -height * 0.5f, -glm::sin(d * i) * radius });
		vertices[getWallVertexIndex(TOP, i)] = glm::vec3({
			glm::cos(d * i) * radius, height * 0.5f, -glm::sin(d * i) * radius });
		normals[getWallNormTangIndex(i)] = glm::vec3({
			glm::cos(d * i), 0.0f, -glm::sin(d * i) });
		tangents[getWallNormTangIndex(i)] = normalToTangentRotation * glm::vec4(normals[i], 0);

		uv[getWallUVIndex(BOTTOM, i)] = { ((float)i) / netDivisions, 0.0f };
		uv[getWallUVIndex(TOP, i)] = { ((float)i) / netDivisions, 1.0f };
		uv[getCircleEdgeUVIndex(BOTTOM, i)] = 0.5f * glm::vec2({ 1 + glm::cos(d * i), 1 - glm::sin(d * i) });
		uv[getCircleEdgeUVIndex(TOP, i)] = 0.5f * glm::vec2({ 1 + glm::cos(d*i), 1 + glm::sin(d * i) });


		

	}
	uv[getWallUVIndex(BOTTOM, netDivisions)] = { 1.0f, 0.0f };
	uv[getWallUVIndex(TOP, netDivisions)] = { 1.0f, 1.0f };
	uv[getCircleEdgeUVIndex(BOTTOM, netDivisions)] = uv[getCircleEdgeUVIndex(BOTTOM, 0)];
	uv[getCircleEdgeUVIndex(TOP, netDivisions)] = uv[getCircleEdgeUVIndex(TOP, 0)];
}

void CyllinderMeshGenerator::setTriangles()
{
	for (int i = 0; i < netDivisions; i++)
	{
		//vertical wall triangles
		triangles[getWallTriangleIndex(i, 0)] = {
			getWallVertexIndex(BOTTOM, i),
			getWallVertexIndex(BOTTOM, i + 1),
			getWallVertexIndex(TOP, i + 1) };
		triangles[getWallTriangleIndex(i, 1)] = {
			getWallVertexIndex(BOTTOM, i),
			getWallVertexIndex(TOP, i + 1),
			getWallVertexIndex(TOP, i) };

		trianglesNormals[getWallTriangleIndex(i, 0)] = {
			getWallNormTangIndex(i),
			getWallNormTangIndex(i + 1),
			getWallNormTangIndex(i + 1) };
		trianglesNormals[getWallTriangleIndex(i, 1)] = {
			getWallNormTangIndex(i),
			getWallNormTangIndex(i + 1),
			getWallNormTangIndex(i) };

		trianglesUV[getWallTriangleIndex(i, 0)] = {
			getWallUVIndex(BOTTOM, i),
			getWallUVIndex(BOTTOM, i + 1),
			getWallUVIndex(TOP, i + 1) };
		trianglesUV[getWallTriangleIndex(i, 1)] = {
			getWallUVIndex(BOTTOM, i),
			getWallUVIndex(TOP, i + 1),
			getWallUVIndex(TOP, i) };

		//bottom triangles
		triangles[getCircleTriangleIndex(BOTTOM, i)] = {
			getCenterVertexIndex(BOTTOM),
			getWallVertexIndex(BOTTOM, i + 1),
			getWallVertexIndex(BOTTOM, i) };
		trianglesNormals[getCircleTriangleIndex(BOTTOM, i)] = {
			getCenterNormTangIndex(BOTTOM),
			getCenterNormTangIndex(BOTTOM),
			getCenterNormTangIndex(BOTTOM) };
		trianglesUV[getCircleTriangleIndex(BOTTOM, i)] = {
			getCenterUVIndex(BOTTOM),
			getCircleEdgeUVIndex(BOTTOM, i + 1),
			getCircleEdgeUVIndex(BOTTOM, i) };

		//top triangles
		triangles[getCircleTriangleIndex(TOP, i)] = {
			getCenterVertexIndex(TOP),
			getWallVertexIndex(TOP, i),
			getWallVertexIndex(TOP, i + 1) };
		trianglesNormals[getCircleTriangleIndex(TOP, i)] = {
			getCenterNormTangIndex(TOP),
			getCenterNormTangIndex(TOP),
			getCenterNormTangIndex(TOP) };
		trianglesUV[getCircleTriangleIndex(TOP, i)] = {
			getCenterUVIndex(TOP),
			getCircleEdgeUVIndex(TOP, i),
			getCircleEdgeUVIndex(TOP, i + 1) };
	}
}

inline int CyllinderMeshGenerator::getCenterVertexIndex(int topOrBottom)
{
	return netDivisions * 2 + topOrBottom;
}

inline int CyllinderMeshGenerator::getCenterNormTangIndex(int topOrBottom)
{
	return netDivisions + topOrBottom;
}

inline int CyllinderMeshGenerator::getWallVertexIndex(int topOrBottom, int wallId)
{
	return topOrBottom * netDivisions + (wallId % netDivisions);
}

inline int CyllinderMeshGenerator::getWallNormTangIndex(int wallId)
{
	return wallId % netDivisions;
}

inline int CyllinderMeshGenerator::getWallUVIndex(int topOrBottom, int wallId)
{
	return (netDivisions + 1) * topOrBottom + wallId;
}

inline int CyllinderMeshGenerator::getCircleEdgeUVIndex(int topOrBottom, int i)
{
	return (netDivisions + 1) * (2 + topOrBottom) + i % netDivisions;
}

inline int CyllinderMeshGenerator::getCenterUVIndex(int topOrBottom)
{
	return (netDivisions + 1) * (2 + topOrBottom) + netDivisions;
}

inline int CyllinderMeshGenerator::getWallTriangleIndex(int wallId, int triangleInPair)
{
	return wallId + netDivisions * triangleInPair;
}

inline int CyllinderMeshGenerator::getCircleTriangleIndex(int topOrBottom, int onCircleId)
{
	return onCircleId + netDivisions * (2 + topOrBottom);
}
