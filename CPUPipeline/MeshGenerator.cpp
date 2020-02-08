#include "MeshGenerator.h"
#include "TransformationMatrices.h"
#include "SphereMeshGenerator.h"
#include "CuboidMeshGenerator.h"
Mesh MeshGenerator::getCuboidMesh(float width, float height, float depth)
{
	CuboidMeshGenerator generator(width, height, depth);
	return generator.getMesh();
}

Mesh MeshGenerator::getCylinderMesh(float H, float R, int netDivisions)
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
	vertices[netDivisions * 2].y *= H * 0.5f;
	tangents[netDivisions] = glm::vec3(1.0f, 0.0f, 0.0f);
	normals[netDivisions + 1]
		= vertices[netDivisions * 2 + 1]
		= glm::vec3(0.0f, 1.0f, 0.0f);//top center
	vertices[netDivisions * 2 + 1].y *= H * 0.5f;
	tangents[netDivisions + 1] = glm::vec3(-1.0f, 0.0f, 0.0f);


	float d = 2.0f * 3.1415f / netDivisions;
	glm::mat4 normalToTangentRotation =
		TransformationMatrices::getRotationMatrix(0.5f * 3.1415f, glm::vec3({ 0.0f,1.0f,0.0f }));
	for (int i = 0; i < netDivisions; i++)
	{
		vertices[i] = glm::vec3({
			glm::cos(d * i)*R, -H*0.5f, -glm::sin(d * i)*R });
		vertices[i+netDivisions] = glm::vec3({
			glm::cos(d * i)* R, H * 0.5f, -glm::sin(d * i) * R });
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
	SphereMeshGenerator generator(R, netVerticalLinesCount, netHorizontalLinesCount);
	return generator.getMesh();
}

Mesh MeshGenerator::getConeMesh(float H, float R, int netDivisions)
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
	//same uvs used for walls (bottom and top ring) and bottom circle
	//at the left edge of the texture u must be one so it can't be the same as the start,
	//two more points need to be defined
	uv.resize(3 * (netDivisions + 1));

	vertices[netDivisions * 2] = glm::vec3(0.0f, -H * 0.5f, 0.0f);//bottom center
	vertices[netDivisions * 2 + 1] = glm::vec3(0.0f, H * 0.5f, 0.0f);//top center
	normals[netDivisions]
		= glm::vec3(0.0f, -1.0f, 0.0f);//bottom center
	tangents[netDivisions] = glm::vec3(1.0f, 0.0f, 0.0f);


	float d = 2.0f * 3.1415f / netDivisions;
	for (int i = 0; i < netDivisions; i++)
	{
		//bottom circle
		vertices[i] = glm::vec3({
			glm::cos(d * i) * R, -H * 0.5f, -glm::sin(d * i) * R });
		//middle circle
		vertices[i + netDivisions] = glm::vec3({
			glm::cos(d * i) * R * 0.5f, 0.0f, -glm::sin(d * i) * R * 0.5f });
		glm::vec3 binormal = glm::normalize(vertices[netDivisions * 2 + 1] - vertices[i]);
		//derivative with respect to d * i
		tangents[i] = glm::normalize(glm::vec3(-glm::sin(d*i), 0.0f, -glm::cos(d*i)));
		normals[i] = glm::normalize(glm::cross(tangents[i], binormal));

		uv[i] = { ((float)i) / netDivisions, 0.0f };
		uv[i + (netDivisions + 1)] = { ((float)i) / netDivisions, 0.5f };
		uv[i + (netDivisions + 1) * 2] = { ((float)i) / (netDivisions-1), 1.0f };


		//between bottom and middle wall triangles
		triangles[i]
			= { i, (i + 1) % netDivisions, (i + 1) % netDivisions + netDivisions };
		triangles[i + netDivisions]
			= { i, (i + 1) % netDivisions + netDivisions, i + netDivisions };

		trianglesNormals[i]
			= { i, (i + 1) % netDivisions, (i + 1) % netDivisions };
		trianglesNormals[i + netDivisions]
			= { i, (i + 1) % netDivisions, i };

		trianglesUV[i]
			= { i, i + 1, (i + 1) + (netDivisions + 1) };
		trianglesUV[i + netDivisions]
			= { i, (i + 1) + (netDivisions + 1), i + (netDivisions + 1) };
	//	//bottom triangles
		triangles[i + netDivisions * 2]
			= { netDivisions * 2,(i + 1) % netDivisions, i };
		trianglesNormals[i + netDivisions * 2]
			= { netDivisions, netDivisions, netDivisions };
		trianglesUV[i + netDivisions * 2]
			= { i, (i + 1) + (netDivisions + 1) * 2, i + (netDivisions + 1) * 2 };

		//between middle and top triangles
		triangles[i + netDivisions * 3]
			= { netDivisions * 2 + 1,i + netDivisions, (i + 1) % netDivisions + netDivisions };
		trianglesNormals[i + netDivisions * 3]
			= { i, (i + 1) % netDivisions, (i + 1) % netDivisions };
		trianglesUV[i + netDivisions * 3]
			= { i + (netDivisions + 1) * 2, i + (netDivisions + 1), (i + 1) + (netDivisions + 1) };

	}
	uv[(netDivisions + 1) * 1 - 1] = { 1.0f, 0.0f };
	uv[(netDivisions + 1) * 2 - 1] = { 1.0f, 0.5f };
	uv[(netDivisions + 1) * 3 - 1] = { 1.0f, 1.0f };

	std::vector<glm::vec2> abc = { uv[trianglesUV[0].x],uv[trianglesUV[0].y] ,uv[trianglesUV[0].z],
		uv[trianglesUV[netDivisions].x],uv[trianglesUV[netDivisions].y] ,uv[trianglesUV[netDivisions].z]
	};

	resMesh.setVertices(vertices);
	resMesh.setNormals(normals);
	resMesh.setTangents(tangents);
	resMesh.setTriangles(triangles);
	resMesh.setTrianglesNormals(trianglesNormals);
	resMesh.setTrianglesUV(trianglesUV);
	resMesh.setUV(uv);
	return resMesh;
}
