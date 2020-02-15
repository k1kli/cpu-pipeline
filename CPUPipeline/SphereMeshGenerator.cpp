#include "SphereMeshGenerator.h"


SphereMeshGenerator::SphereMeshGenerator(float R, int netVerticalLinesCount, int netHorizontalLinesCount)
	:SphereMeshGenerator()
{
	parameters = { R, (float)netVerticalLinesCount, (float)netHorizontalLinesCount };
}
SphereMeshGenerator::SphereMeshGenerator()
	:VirtualMeshGenerator({ "radius", "vertical lines in net", "horizontal lines in net" }, "sphere")
{
}
std::shared_ptr<VirtualMeshGenerator> SphereMeshGenerator::getInstance() const
{
	return std::shared_ptr<VirtualMeshGenerator>(new SphereMeshGenerator(*this));
}

void SphereMeshGenerator::validateParameters()
{
	R = parameters[0];
	netHorizontalLinesCount = parameters[1];
	netVerticalLinesCount = parameters[2];
	if (R <= 0.0f) throw "sphere radius should be positive";
	if (netHorizontalLinesCount <= 0) throw "number of horizontal lines should be positive";
	if (netVerticalLinesCount <= 2) throw "number of vertical lines should be higher than 2";
}

void SphereMeshGenerator::resizeVectors()
{
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
}

void SphereMeshGenerator::setValues()
{
	//non pole vertices

	//south pole is at pi, first horizontal line is at p1-d1, last horizontal line is at d1,
	//north pole is at 0
	float d1 = 3.1415f / (netHorizontalLinesCount + 1);
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
			int i = getNonPoleVertNormTangIndex(horizontalLine, verticalLine);
			normals[i]
				= glm::vec3(sin(alpha) * cos(beta), cos(alpha), -sin(alpha) * sin(beta));
			vertices[i]
				= normals[i] * R;

			//derivative with respect to beta
			tangents[i]
				= glm::normalize(
					glm::vec3(-sin(alpha) * sin(beta), cos(alpha), -sin(alpha) * cos(beta)));

			uv[getNonPoleUVIndex(horizontalLine, verticalLine)] = glm::vec2(
				(float)verticalLine / netVerticalLinesCount,
				(float)(horizontalLine + 1) / (netHorizontalLinesCount + 1));
		}
		uv[getNonPoleUVIndex(horizontalLine, netVerticalLinesCount)] = glm::vec2(
			1.0f, (float)(horizontalLine + 1) / (netHorizontalLinesCount + 1));
	}


	//pole vertices
	vertices[getPoleVertexIndex(SOUTH_POLE)] = glm::vec3(0.0f, -R, 0.0f);
	vertices[getPoleVertexIndex(NORTH_POLE)] = glm::vec3(0.0f, R, 0.0f);
	for (int touchingTriangle = 0;
		touchingTriangle < netVerticalLinesCount; touchingTriangle++)
	{
		//south pole
		int i = getPoleNormTangIndex(SOUTH_POLE, touchingTriangle);
		normals[i]
			= glm::vec3(0.0f, -1.0, 0.0f);
		float alpha = d1 * (touchingTriangle + 0.5f);
		tangents[i] = glm::normalize(glm::vec3(cos(alpha), 0.0f, -sin(alpha)));
		uv[getPoleUVIndex(SOUTH_POLE, touchingTriangle)]
			= glm::vec2((touchingTriangle + 0.5f) / netVerticalLinesCount, 0.0f);



		//north pole
		i = getPoleNormTangIndex(NORTH_POLE, touchingTriangle);
		normals[i]
			= glm::vec3(0.0f, 1.0f, 0.0f);
		alpha = d1 * (touchingTriangle + 0.5f);
		tangents[i] = glm::normalize(glm::vec3(cos(alpha), 0.0f, -sin(alpha)));
		uv[getPoleUVIndex(NORTH_POLE, touchingTriangle)]
			= glm::vec2((touchingTriangle + 0.5f) / netVerticalLinesCount, 1.0f);
	}
}

void SphereMeshGenerator::setTriangles()
{
	//triangles

	//non pole triangles
	for (int horizontalLine = 0; horizontalLine < netHorizontalLinesCount - 1; horizontalLine++)
	{
		//making triangles above horizontalLine
		for (int verticalLine = 0; verticalLine < netVerticalLinesCount; verticalLine++)
		{
			int i = getNonPoleTriangleIndex(horizontalLine, verticalLine, 0);
			triangles[i] = glm::uvec3(
				getNonPoleVertNormTangIndex(horizontalLine, verticalLine),
				getNonPoleVertNormTangIndex(horizontalLine, verticalLine + 1),
				getNonPoleVertNormTangIndex(horizontalLine + 1, verticalLine + 1));
			trianglesNormals[i] = triangles[i];
			trianglesUV[i] = glm::uvec3(
				getNonPoleUVIndex(horizontalLine, verticalLine),
				getNonPoleUVIndex(horizontalLine, verticalLine + 1),
				getNonPoleUVIndex(horizontalLine + 1, verticalLine + 1));

			i = getNonPoleTriangleIndex(horizontalLine, verticalLine, 1);

			triangles[i] = glm::uvec3(
				getNonPoleVertNormTangIndex(horizontalLine, verticalLine),
				getNonPoleVertNormTangIndex(horizontalLine + 1, verticalLine + 1),
				getNonPoleVertNormTangIndex(horizontalLine + 1, verticalLine));
			trianglesNormals[i] = triangles[i];
			trianglesUV[i] = glm::uvec3(
				getNonPoleUVIndex(horizontalLine, verticalLine),
				getNonPoleUVIndex(horizontalLine + 1, verticalLine + 1),
				getNonPoleUVIndex(horizontalLine + 1, verticalLine));
		}
	}

	//pole triangles
	for (int touchingTriangle = 0;
		touchingTriangle < netVerticalLinesCount; touchingTriangle++)
	{
		int i = getPoleTriangleIndex(SOUTH_POLE, touchingTriangle);
		triangles[i] = glm::uvec3(
			getPoleVertexIndex(SOUTH_POLE),
			getNonPoleVertNormTangIndex(0, touchingTriangle + 1),
			getNonPoleVertNormTangIndex(0, touchingTriangle));
		trianglesNormals[i] = glm::uvec3(
			getPoleNormTangIndex(SOUTH_POLE, touchingTriangle),
			getNonPoleVertNormTangIndex(0, touchingTriangle + 1),
			getNonPoleVertNormTangIndex(0, touchingTriangle));
		trianglesUV[i] = glm::uvec3(
			getPoleUVIndex(SOUTH_POLE, touchingTriangle),
			getNonPoleUVIndex(0, touchingTriangle + 1),
			getNonPoleUVIndex(0, touchingTriangle));

		i = getPoleTriangleIndex(NORTH_POLE, touchingTriangle);

		triangles[i] = glm::uvec3(
			getPoleVertexIndex(NORTH_POLE),
			getNonPoleVertNormTangIndex(netHorizontalLinesCount - 1, touchingTriangle),
			getNonPoleVertNormTangIndex(netHorizontalLinesCount - 1, touchingTriangle + 1));
		trianglesNormals[i] = glm::uvec3(
			getPoleNormTangIndex(NORTH_POLE, touchingTriangle),
			getNonPoleVertNormTangIndex(netHorizontalLinesCount - 1, touchingTriangle),
			getNonPoleVertNormTangIndex(netHorizontalLinesCount - 1, touchingTriangle + 1));
		trianglesUV[i] = glm::uvec3(
			getPoleUVIndex(NORTH_POLE, touchingTriangle),
			getNonPoleUVIndex(netHorizontalLinesCount - 1, touchingTriangle),
			getNonPoleUVIndex(netHorizontalLinesCount - 1, touchingTriangle + 1));
	}
}




inline int SphereMeshGenerator::getNonPoleVertNormTangIndex(int horizontalLine, int verticalLine)
{
	return horizontalLine * netVerticalLinesCount + verticalLine % netVerticalLinesCount;
}

inline int SphereMeshGenerator::getPoleVertexIndex(int pole)
{
	return netHorizontalLinesCount * netVerticalLinesCount + pole;
}

inline int SphereMeshGenerator::getPoleNormTangIndex(int pole, int touchingTriangleId)
{
	return netHorizontalLinesCount * netVerticalLinesCount
		+ pole * netVerticalLinesCount + touchingTriangleId;
}

inline int SphereMeshGenerator::getNonPoleTriangleIndex(int horizontalLine, int verticalLine, int triangleInPair)
{
	return horizontalLine * netVerticalLinesCount + verticalLine
		+ netHorizontalLinesCount * netVerticalLinesCount * triangleInPair;
}

inline int SphereMeshGenerator::getPoleTriangleIndex(int pole, int touchingTriangleId)
{
	return (netHorizontalLinesCount - 1) * netVerticalLinesCount
		+ netHorizontalLinesCount * netVerticalLinesCount * pole
		+ touchingTriangleId;
}
inline int SphereMeshGenerator::getNonPoleUVIndex(int horizontalLine, int verticalLine)
{
	return horizontalLine * (netVerticalLinesCount + 1) + verticalLine;
}
inline int SphereMeshGenerator::getPoleUVIndex(int pole, int touchingTriangleId)
{
	return netHorizontalLinesCount * (netVerticalLinesCount + 1)
		+ netVerticalLinesCount * pole
		+ touchingTriangleId;
}



