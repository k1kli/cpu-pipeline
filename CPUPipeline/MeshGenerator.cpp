#include "MeshGenerator.h"
#include "TransformationMatrices.h"
Mesh MeshGenerator::getCubeMesh()
{
	Mesh resMesh;
	resMesh.setVertices({
		   {0.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f}, {1.0f,1.0f,0.0f}, {1.0f,0.0f,0.0f},
		   {0.0f,0.0f,1.0f}, {0.0f,1.0f,1.0f}, {1.0f,1.0f,1.0f}, {1.0f,0.0f,1.0f}
		});
	//front, back, right, left, up, down
	resMesh.setNormals({
		{0.0f,0.0f,1.0f},{0.0f,0.0f,-1.0f},
		{1.0f,0.0f,0.0f},{-1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},{0.0f,-1.0f,0.0f},
		});
	resMesh.setTangents({
		{1.0f,0.0f,0.0f},{-1.0f,0.0f,0.0f},
		{0.0f,0.0f,-1.0f},{0.0f,0.0f,1.0f},
		{1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f}
		});
	//cube:
	//right handed mesh
	//back:
	//1 2
	//0 3

	//front:
	//5 6
	//4 7
	resMesh.setTriangles({
			{4,7,6},{4,6,5},//front
			{3,0,1},{3,1,2},//back
			{7,3,2},{7,2,6},//right
			{0,4,5},{0,5,1},//left
			{5,6,2},{5,2,1},//up
			{0,7,4},{0,3,7}//down
		});
	resMesh.setTrianglesNormals({
			{0,0,0},{0,0,0},//front
			{1,1,1},{1,1,1},//back
			{2,2,2},{2,2,2},//right
			{3,3,3},{3,3,3},//left
			{4,4,4},{4,4,4},//up
			{5,5,5},{5,5,5}//down
		});
	resMesh.setUV({
		{0,0},//bottom left
		{0,1},//top left
		{1,0},//bottom right
		{1,1}//top right
		});
	resMesh.setTrianglesUV({
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,2,3},{0,3,1},
		{0,3,1},{0,2,3}
		});
	return resMesh;
}

Mesh MeshGenerator::getCylinderMesh(int netDivisions)
{
	if (netDivisions < 3) throw "net divisions should be higher than 2";
	Mesh resMesh;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::uvec3> triangles;
	std::vector<glm::uvec3> trianglesNormals;
	std::vector<glm::vec2> uv;
	std::vector<glm::uvec3> trianglesUV;
	//top and bottom centers and two vertices on left of each side
	vertices.resize(2 + netDivisions * 2);

	//top and bottom and one for each vertical edge
	normals.resize(2 + netDivisions);
	tangents.resize(2 + netDivisions);

	//two for each vertical wall and netDivisions for top and bottom
	triangles.resize(2 * netDivisions + 2 * netDivisions);
	trianglesNormals.resize(2 * netDivisions + 2 * netDivisions);
	trianglesUV.resize(2 * netDivisions + 2 * netDivisions);

	//same uvs used for walls and top and bottom
	//at the left edge of the texture u must be one so it can't be the same as the start,
	//two more points need to be defined
	uv.resize(2 * netDivisions+2);


	normals[netDivisions]
		= vertices[netDivisions * 2]
		= glm::vec3(0.0f, -1.0f, 0.0f);//bottom center
	tangents[netDivisions] = glm::vec3(1.0f, 0.0f, 0.0f);
	normals[netDivisions + 1]
		= vertices[netDivisions * 2 + 1]
		= glm::vec3(0.0f, 1.0f, 0.0f);//top center
	tangents[netDivisions + 1] = glm::vec3(-1.0f, 0.0f, 0.0f);


	float d = 2.0f * 3.1415f / netDivisions;
	glm::mat4 normalToTangentRotation =
		TransformationMatrices::getRotationMatrix(0.5f * 3.1415f, glm::vec3({ 0.0f,1.0f,0.0f }));
	for (int i = 0; i < netDivisions; i++)
	{
		vertices[i] = glm::vec3({
			glm::cos(d * i), -1.0f, -glm::sin(d * i) });
		vertices[i+netDivisions] = glm::vec3({
			glm::cos(d * i), 1.0f, -glm::sin(d * i) });
		normals[i] = glm::vec3({
			glm::cos(d * i), 0.0f, -glm::sin(d * i) });
		tangents[i] = normalToTangentRotation * glm::vec4(normals[i],0);

		uv[i] = { ((float)i) / netDivisions, 0.0f };
		uv[i + (netDivisions + 1)] = { ((float)i) / netDivisions, 1.0f };


		//vertical wall triangles
		triangles[i]
			= { i, (i + 1) % netDivisions, (i + 1) % netDivisions + netDivisions };
		triangles[i+netDivisions]
			= { i, (i + 1) % netDivisions + netDivisions, i + netDivisions };

		trianglesNormals[i]
			= { i, (i + 1) % netDivisions, (i + 1) % netDivisions };
		trianglesNormals[i+netDivisions] 
			= { i, (i + 1) % netDivisions, i };

		trianglesUV[i]
			= { i, i + 1, (i + 1) + (netDivisions + 1) };
		trianglesUV[i + netDivisions]
			= { i, (i + 1) + (netDivisions+1), i + (netDivisions+1) };
		//bottom triangles
		triangles[i + netDivisions * 2]
			= { netDivisions * 2,(i + 1) % netDivisions, i };
		trianglesNormals[i + netDivisions * 2]
			= { netDivisions, netDivisions, netDivisions };
		trianglesUV[i + netDivisions * 2]
			= { i, (i + 1) + (netDivisions + 1), i + (netDivisions + 1) };

		//top triangles
		triangles[i + netDivisions * 3]
			= { netDivisions * 2 + 1,i + netDivisions, (i + 1) % netDivisions + netDivisions };
		trianglesNormals[i + netDivisions * 3]
			= { netDivisions + 1, netDivisions + 1, netDivisions + 1 };
		trianglesUV[i + netDivisions * 3]
			= { i, i + (netDivisions + 1), (i + 1) + (netDivisions + 1) };

	}
	uv[netDivisions] = { 1.0f, 0.0f };
	uv[netDivisions*2+1] = { 1.0f, 1.0f };


	resMesh.setVertices(vertices);
	resMesh.setNormals(normals);
	resMesh.setTangents(tangents);
	resMesh.setTriangles(triangles);
	resMesh.setTrianglesNormals(trianglesNormals);
	resMesh.setTrianglesUV(trianglesUV);
	resMesh.setUV(uv);
	return resMesh;
}

Mesh MeshGenerator::getSphereMesh(float R, int netVerticalLinesCount, int netHorizontalLinesCount)
{
	if (R <= 0.0f) throw "sphere radius should be positive";
	if (netHorizontalLinesCount <= 0) throw "number of horizontal lines should be positive";
	if (netVerticalLinesCount <= 2) throw "number of vertical lines should be higher than 2";
	Mesh resMesh;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::uvec3> triangles;
	std::vector<glm::uvec3> trianglesNormals;
	std::vector<glm::vec2> uv;
	std::vector<glm::uvec3> trianglesUV;
	//vertex on every net division and on top and bottom
	vertices.resize(2 + netHorizontalLinesCount * netVerticalLinesCount);

	//normal and tangent for every non pole vertex
	//for each pole one normal and tangent for each triangle touching that pole
	normals.resize(2 * netVerticalLinesCount + netHorizontalLinesCount * netVerticalLinesCount);
	tangents.resize(2 * netVerticalLinesCount + netHorizontalLinesCount * netVerticalLinesCount);

	//every horizontal line consists of triangles 
	//bases of (netVerticalLinesCount) triangles above it
	//and (netVerticalLinesCount) triangles below it
	//first non pole triangles then triangles next to south pole, then north pole
	triangles.resize(netHorizontalLinesCount * netVerticalLinesCount * 2);
	trianglesNormals.resize(netHorizontalLinesCount * netVerticalLinesCount * 2);
	trianglesUV.resize(netHorizontalLinesCount * netVerticalLinesCount * 2);

	//for vertices not at the top or bottom one uv defined,
	//except for those at left and right edge of texture where two for each vertex are defined
	//at the top and bottom one uv for each triangle connected
	uv.resize(netHorizontalLinesCount * netVerticalLinesCount
		+ netHorizontalLinesCount + netVerticalLinesCount * 2);



	//non pole vertices

	//south pole is at pi, first horizontal line is at p1-d1, last horizontal line is at d1,
	//north pole is at 0
	float d1 = 3.1415f / (netHorizontalLinesCount+1);
	float d2 = 2.0f * 3.1415f / netVerticalLinesCount;
	float alpha = 3.1415f - d1;
	for (int horizontalLine = 0;
		horizontalLine < netHorizontalLinesCount;
		horizontalLine++, alpha -= d1)
	{
		float beta = 0.0f;
		for (int verticalLine = 0; verticalLine < netVerticalLinesCount;
			verticalLine++, beta += d2)
		{
			int i = horizontalLine * netVerticalLinesCount + verticalLine;
			normals[i]
				= glm::vec3(sin(alpha) * cos(beta), cos(alpha), -sin(alpha) * sin(beta));
			vertices[i]
				= normals[i] * R;
			tangents[i] = glm::normalize(
				glm::vec3(-sin(alpha) * sin(beta), cos(alpha), -sin(alpha) * cos(beta)));
			uv[horizontalLine * (netVerticalLinesCount+1) + verticalLine] = glm::vec2(
				(float)verticalLine/netVerticalLinesCount,
				(float)(horizontalLine+1)/(netHorizontalLinesCount+1));
		}
		uv[horizontalLine * (netVerticalLinesCount + 1) + netVerticalLinesCount] = glm::vec2(
			1.0f, (float)(horizontalLine + 1) / (netHorizontalLinesCount + 1));
	}


	//pole vertices
	vertices[netHorizontalLinesCount * netVerticalLinesCount] = glm::vec3(0.0f, -R, 0.0f);
	vertices[netHorizontalLinesCount * netVerticalLinesCount + 1] = glm::vec3(0.0f, R, 0.0f);
	for (int touchingTriangle = 0;
		touchingTriangle < netVerticalLinesCount; touchingTriangle++)
	{
		//south pole
		int i = netHorizontalLinesCount * netVerticalLinesCount + touchingTriangle;
		int uvi = netHorizontalLinesCount * (netVerticalLinesCount + 1) + touchingTriangle;
		normals[i]
			= glm::vec3(0.0f, -1.0, 0.0f);
		float alpha = d1 * (touchingTriangle + 0.5f);
		tangents[i] = glm::normalize(glm::vec3(cos(alpha), 0.0f, -sin(alpha)));
		uv[uvi] = glm::vec2((float)touchingTriangle/ netVerticalLinesCount, 0.0f);

		i += netVerticalLinesCount;
		uvi += netVerticalLinesCount;

		//north pole
		normals[i]
			= glm::vec3(0.0f, 1.0f, 0.0f);
		alpha = d1 * (touchingTriangle + 0.5f);
		tangents[i] = glm::normalize(glm::vec3(cos(alpha), 0.0f, -sin(alpha)));
		uv[uvi] = glm::vec2((float)touchingTriangle / netVerticalLinesCount, 1.0f);
	}


	//triangles

	//non pole triangles
	for (int horizontalLine = 0; horizontalLine < netHorizontalLinesCount-1; horizontalLine++)
	{
		//making triangles above horizontalLine
		for (int verticalLine = 0; verticalLine < netVerticalLinesCount; verticalLine++)
		{
			int i = horizontalLine * netVerticalLinesCount + verticalLine;
			triangles[i] = glm::uvec3(
				horizontalLine * netVerticalLinesCount + verticalLine,
				horizontalLine * netVerticalLinesCount + ((verticalLine + 1) % netVerticalLinesCount),
				(horizontalLine + 1) * netVerticalLinesCount
				+ ((verticalLine + 1) % netVerticalLinesCount));
			trianglesNormals[i] = triangles[i];
			trianglesUV[i] = glm::uvec3(
					horizontalLine * (netVerticalLinesCount + 1) + verticalLine,
					horizontalLine * (netVerticalLinesCount + 1) + (verticalLine + 1),
					(horizontalLine + 1) * (netVerticalLinesCount + 1)
					+ (verticalLine + 1));

			i += netHorizontalLinesCount * netVerticalLinesCount;

			triangles[i] = glm::uvec3(
				horizontalLine * netVerticalLinesCount + verticalLine,
				(horizontalLine + 1) * netVerticalLinesCount
				+ ((verticalLine + 1) % netVerticalLinesCount),
				(horizontalLine + 1) * netVerticalLinesCount + verticalLine);
			trianglesNormals[i] = triangles[i];
			trianglesUV[i] = glm::uvec3(
				horizontalLine * (netVerticalLinesCount + 1) + verticalLine,
				(horizontalLine+1) * (netVerticalLinesCount + 1) + (verticalLine + 1),
				(horizontalLine + 1) * (netVerticalLinesCount + 1)+verticalLine);
		}
	}

	//pole triangles
	for (int touchingTriangle = 0;
		touchingTriangle < netVerticalLinesCount; touchingTriangle++)
	{
		int i = (netHorizontalLinesCount - 1) * netVerticalLinesCount + touchingTriangle;
		triangles[i] = glm::uvec3(
			netHorizontalLinesCount * netVerticalLinesCount,
			(touchingTriangle + 1) % netVerticalLinesCount,
			touchingTriangle);
		trianglesNormals[i] = glm::uvec3(
			netHorizontalLinesCount * netVerticalLinesCount + touchingTriangle,
			(touchingTriangle + 1) % netVerticalLinesCount,
			touchingTriangle);
		trianglesUV[i] = glm::uvec3(
			netHorizontalLinesCount * (netVerticalLinesCount+1) + touchingTriangle,
			touchingTriangle + 1,
			touchingTriangle);

		i += netVerticalLinesCount*netHorizontalLinesCount;

		triangles[i] = glm::uvec3(
			netHorizontalLinesCount * netVerticalLinesCount + 1,
			(netHorizontalLinesCount-1) * netVerticalLinesCount
			+ touchingTriangle,
			(netHorizontalLinesCount - 1) * netVerticalLinesCount
			+ (touchingTriangle + 1) % netVerticalLinesCount);
		trianglesNormals[i] = glm::uvec3(
			(netHorizontalLinesCount+1) * netVerticalLinesCount + touchingTriangle,
			(netHorizontalLinesCount - 1) * netVerticalLinesCount
			+ touchingTriangle,
			(netHorizontalLinesCount - 1) * netVerticalLinesCount
			+ (touchingTriangle + 1) % netVerticalLinesCount);
		trianglesUV[i] = glm::uvec3(
			netHorizontalLinesCount * (netVerticalLinesCount + 1)
			+ netVerticalLinesCount + touchingTriangle,
			(netHorizontalLinesCount - 1) * (netVerticalLinesCount+1)
			+ touchingTriangle,
			(netHorizontalLinesCount - 1) * (netVerticalLinesCount+1)
			+ touchingTriangle + 1);
	}
	


	resMesh.setVertices(vertices);
	resMesh.setNormals(normals);
	resMesh.setTangents(tangents);
	resMesh.setTriangles(triangles);
	resMesh.setTrianglesNormals(trianglesNormals);
	resMesh.setTrianglesUV(trianglesUV);
	resMesh.setUV(uv);
	return resMesh;
}
