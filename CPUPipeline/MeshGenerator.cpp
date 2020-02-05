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
	tangents[netDivisions] = glm::vec3(-1.0f, 0.0f, 0.0f);


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
