#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <functional>
struct ClipPolygon
{
	int verticesCount = 0;
	glm::vec4 vertices[12];

};
class TriangleClipper
{
	std::vector<std::function<float(const glm::vec4&)>> edgeDist =
	{
		[](const glm::vec4& point) {return point.w - point.x; },
		[](const glm::vec4& point) {return point.w + point.x; },
		[](const glm::vec4& point) {return point.w - point.y; },
		[](const glm::vec4& point) {return point.w + point.y; },
		[](const glm::vec4& point) {return point.w - point.z; },
		[](const glm::vec4& point) {return point.w + point.z; }
	};
	void ClipAgainst(int edgeId, const ClipPolygon& inVertices, ClipPolygon& outVertices);
public:
	void ClipTriangle(std::vector<glm::vec4>& vertices);
	bool isPointVisible(const glm::vec4& point) const;
};

