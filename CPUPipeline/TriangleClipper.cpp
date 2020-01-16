#include "TriangleClipper.h"


void TriangleClipper::ClipTriangle(std::vector<glm::vec4>& vertices)
{
	ClipPolygon poly1;
	ClipPolygon poly2;
	for (int i = 0; i < vertices.size(); i++)
	{
		poly1.vertices[poly1.verticesCount++] = vertices[i];
	}
	ClipAgainst(0, poly1, poly2);
	poly1.verticesCount = 0;
	ClipAgainst(1, poly2, poly1);
	poly2.verticesCount = 0;
	ClipAgainst(2, poly1, poly2);
	poly1.verticesCount = 0;
	ClipAgainst(3, poly2, poly1);
	poly2.verticesCount = 0;
	ClipAgainst(4, poly1, poly2);
	poly1.verticesCount = 0;
	ClipAgainst(5, poly2, poly1);
	vertices.clear();
	for (int i = 0; i < poly1.verticesCount; i++)
	{
		vertices.push_back(poly1.vertices[i]);
	}
}

void TriangleClipper::ClipAgainst(int edgeId, const ClipPolygon& inVertices, ClipPolygon& outVertices)
{
	float dPrev = edgeDist[edgeId](inVertices.vertices[0]);
	for (int i = 1; i < inVertices.verticesCount; i++)
	{
		float dNext = edgeDist[edgeId](inVertices.vertices[i]);
		if (dPrev >= 0)
		{
			if (dNext >= 0)
			{
				outVertices.vertices[outVertices.verticesCount++] = inVertices.vertices[i];
			}
			else
			{
				float dC = dPrev / (dPrev - dNext);
				glm::vec4 C = inVertices.vertices[i-1] * (1 - dC) + inVertices.vertices[i] * dC;
				outVertices.vertices[outVertices.verticesCount++] = C;
			}
		}
		else
		{
			if (dNext >= 0)
			{
				float dC = dPrev / (dPrev - dNext);
				glm::vec4 C = inVertices.vertices[i - 1] * (1 - dC) + inVertices.vertices[i] * dC;
				outVertices.vertices[outVertices.verticesCount++] = C;
				outVertices.vertices[outVertices.verticesCount++] = inVertices.vertices[i];
			}
		}
		dPrev = dNext;
	}
	//last iteration because polygon wraps
	float dNext = edgeDist[edgeId](inVertices.vertices[0]);
	if (dPrev >= 0)
	{
		if (dNext >= 0)
		{
			outVertices.vertices[outVertices.verticesCount++] = inVertices.vertices[0];
		}
		else
		{
			float dC = dPrev / (dPrev - dNext);
			glm::vec4 C = inVertices.vertices[inVertices.verticesCount - 1] * (1 - dC)
				+ inVertices.vertices[0] * dC;
			outVertices.vertices[outVertices.verticesCount++] = C;
		}
	}
	else
	{
		if (dNext >= 0)
		{
			float dC = dPrev / (dPrev - dNext);
			glm::vec4 C = inVertices.vertices[inVertices.verticesCount - 1] * (1 - dC)
				+ inVertices.vertices[0] * dC;
			outVertices.vertices[outVertices.verticesCount++] = C;
			outVertices.vertices[outVertices.verticesCount++] = inVertices.vertices[0];
		}
	}
}