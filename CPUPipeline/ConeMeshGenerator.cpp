#include "ConeMeshGenerator.h"

void ConeMeshGenerator::validateParameters()
{
	if (netDivisions < 3) throw "net divisions should be 3 or higher";
	if (height <= 0) throw "height should be positive";
	if (radius <= 0) throw "radius should be positive";
}

void ConeMeshGenerator::resizeVectors()
{
	//top and bottom centers and netDivisions vertices on bottom and middle circle
	vertices.resize(2 + netDivisions * 2);

	//netDivisions for sides and one for bottom
	normals.resize(netDivisions + 1);
	tangents.resize(netDivisions + 1);

	//2 * netDivisions triangles between bottom and middle, netDivisions triangles between middle and top
	//and netDivisions triangles on bottom circle
	triangles.resize(4 * netDivisions);
	trianglesNormals.resize(4 * netDivisions);
	trianglesUV.resize(4 * netDivisions);
	//(netDivisions+1) uvs for each ring (bottom, middle, top)
	//and for bottom circle: one for each edge vertex and one for center
	uv.resize(4 * (netDivisions + 1));
}

void ConeMeshGenerator::setValues()
{
	vertices[getCenterVertexIndex(BOTTOM)] = glm::vec3(0.0f, -height * 0.5f, 0.0f);
	vertices[getCenterVertexIndex(TOP)] = glm::vec3(0.0f, height * 0.5f, 0.0f);
	normals[getCircleNormTangIndex()]
		= glm::vec3(0.0f, -1.0f, 0.0f);//bottom center
	tangents[getCircleNormTangIndex()] = glm::vec3(1.0f, 0.0f, 0.0f);

	uv[getCircleCenterUVIndex()] = glm::vec2(0.5f, 0.5f);


	float d = 2.0f * 3.1415f / netDivisions;
	for (int i = 0; i < netDivisions; i++)
	{
		//bottom circle
		vertices[getWallVertexIndex(BOTTOM, i)] = glm::vec3({
			glm::cos(d * i) * radius, -height * 0.5f, -glm::sin(d * i) * radius });
		//middle circle
		vertices[getWallVertexIndex(MIDDLE, i)] = glm::vec3({
			glm::cos(d * i) * radius * 0.5f, 0.0f, -glm::sin(d * i) * radius * 0.5f });
		glm::vec3 binormal = glm::normalize(vertices[netDivisions * 2 + 1] - vertices[i]);
		//derivative with respect to d * i
		tangents[getNormTangIndex(i)] = glm::normalize(glm::vec3(-glm::sin(d * i), 0.0f, -glm::cos(d * i)));
		normals[getNormTangIndex(i)] = glm::normalize(glm::cross(tangents[i], binormal));

		uv[getWallUVIndex(BOTTOM, i)] = { ((float)i) / netDivisions, 0.0f };
		uv[getWallUVIndex(MIDDLE, i)] = { ((float)i) / netDivisions, 0.5f };
		uv[getWallUVIndex(TOP, i)] = { ((float)i) / (netDivisions - 1), 1.0f };
		uv[getCircleEdgeUVIndex(i)] = 0.5f * glm::vec2({ 1 + glm::cos(d * i), 1 - glm::sin(d * i) });
	}
	uv[getWallUVIndex(BOTTOM, netDivisions)] = { 1.0f, 0.0f };
	uv[getWallUVIndex(MIDDLE, netDivisions)] = { 1.0f, 0.5f };
	uv[getWallUVIndex(TOP, netDivisions)] = { 1.0f, 1.0f };

}

void ConeMeshGenerator::setTriangles()
{

	for (int i = 0; i < netDivisions; i++)
	{
		//between bottom and middle wall triangles
		triangles[getWallTriangleIndex(BOTTOM, i, 0)] = {
			getWallVertexIndex(BOTTOM, i),
			getWallVertexIndex(BOTTOM, i + 1),
			getWallVertexIndex(MIDDLE, i + 1) };
		triangles[getWallTriangleIndex(BOTTOM, i, 1)] = {
			getWallVertexIndex(BOTTOM, i),
			getWallVertexIndex(MIDDLE, i + 1),
			getWallVertexIndex(MIDDLE, i) };

		trianglesNormals[getWallTriangleIndex(BOTTOM, i, 0)] = {
			getNormTangIndex(i),
			getNormTangIndex(i + 1),
			getNormTangIndex(i + 1) };
		trianglesNormals[getWallTriangleIndex(BOTTOM, i, 1)] = {
			getNormTangIndex(i),
			getNormTangIndex(i + 1),
			getNormTangIndex(i) };

		trianglesUV[getWallTriangleIndex(BOTTOM, i, 0)] = {
			getWallUVIndex(BOTTOM, i),
			getWallUVIndex(BOTTOM, i + 1),
			getWallUVIndex(MIDDLE, i + 1) };
		trianglesUV[getWallTriangleIndex(BOTTOM, i, 1)] = {
			getWallUVIndex(BOTTOM, i),
			getWallUVIndex(MIDDLE, i + 1),
			getWallUVIndex(MIDDLE, i) };
		//	//bottom triangles
		triangles[getCircleTriangleIndex(i)] = {
			getCenterVertexIndex(BOTTOM),
			getWallVertexIndex(BOTTOM, i + 1),
			getWallVertexIndex(BOTTOM, i) };
		trianglesNormals[getCircleTriangleIndex(i)] = {
			getCircleNormTangIndex(),
			getCircleNormTangIndex(),
			getCircleNormTangIndex() };
		trianglesUV[getCircleTriangleIndex(i)] = {
			getCircleCenterUVIndex(),
			getCircleEdgeUVIndex(i + 1),
			getCircleEdgeUVIndex(i) };

		//between middle and top triangles
		triangles[getWallTriangleIndex(MIDDLE, i, -1)] = {
			getCenterVertexIndex(TOP),
			getWallVertexIndex(MIDDLE, i),
			getWallVertexIndex(MIDDLE, i + 1) };
		trianglesNormals[getWallTriangleIndex(MIDDLE, i, -1)] = {
			getNormTangIndex(i),
			getNormTangIndex(i + 1),
			getNormTangIndex(i + 1) };
		trianglesUV[getWallTriangleIndex(MIDDLE, i, -1)] = {
			getWallUVIndex(TOP, i),
			getWallUVIndex(MIDDLE, i),
			getWallUVIndex(MIDDLE, i + 1) };
	}
}

inline int ConeMeshGenerator::getCenterVertexIndex(int topOrBottom)
{
	return netDivisions * 2 + (topOrBottom == TOP ? 1 : 0);
}

inline int ConeMeshGenerator::getWallVertexIndex(int middleOrBottom, int wallId)
{
	return netDivisions * middleOrBottom + wallId % netDivisions;
}

inline int ConeMeshGenerator::getCircleNormTangIndex()
{
	return netDivisions;
}

inline int ConeMeshGenerator::getNormTangIndex(int wallId)
{
	return wallId % netDivisions;
}

inline int ConeMeshGenerator::getWallUVIndex(int topMiddleBottom, int wallId)
{
	return (netDivisions + 1) * topMiddleBottom + wallId;
}

inline int ConeMeshGenerator::getCircleCenterUVIndex()
{
	return netDivisions * 4 + 3;
}

inline int ConeMeshGenerator::getCircleEdgeUVIndex(int triangleId)
{
	return (netDivisions + 1) * 3 + triangleId % netDivisions;
}

inline int ConeMeshGenerator::getWallTriangleIndex(int aboveBottomOrMiddle, int wallId, int triangleInPair)
{
	return aboveBottomOrMiddle == BOTTOM
		? (netDivisions * triangleInPair + wallId)
		: (netDivisions * 2 + wallId);
}

inline int ConeMeshGenerator::getCircleTriangleIndex(int triangleId)
{
	return netDivisions * 3 + triangleId;
}
