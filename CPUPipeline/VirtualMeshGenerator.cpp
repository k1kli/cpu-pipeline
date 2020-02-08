#include "VirtualMeshGenerator.h"

Mesh VirtualMeshGenerator::getMesh()
{
	validateParameters();
	resizeVectors();

	setValues();
	setTriangles();

	resMesh.setVertices(vertices);
	resMesh.setNormals(normals);
	resMesh.setTangents(tangents);
	resMesh.setTriangles(triangles);
	resMesh.setTrianglesNormals(trianglesNormals);
	resMesh.setTrianglesUV(trianglesUV);
	resMesh.setUV(uv);
	return resMesh;
}
