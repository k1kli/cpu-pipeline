#include "FrameBuffer.h"

#include <string>
#include <memory>
#include <algorithm>
#include <glad/glad.h>
#include "shader.hpp"


FrameBuffer::FrameBuffer(int _w, int _h)
	:m_width(_w), m_height(_h)
{
	m_color_buffer = new uint8_t[m_width * m_height * m_bytesPerPixel];
	depthBuffer = new float[m_width * m_height];
}

FrameBuffer::~FrameBuffer()
{
	if (m_color_buffer)
		delete[] m_color_buffer;
	if (depthBuffer)
		delete[] depthBuffer;
}

void FrameBuffer::InitGL()
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_color_buffer);

	shader = Shader::Create("shader", R"(
		#version 330 core

		out vec2 texCoord;
	
		void main()
		{
			float x = -1.0 + float((gl_VertexID & 1) << 2);
			float y = -1.0 + float((gl_VertexID & 2) << 1);
			texCoord.x = (x+1.0)*0.5;
			texCoord.y = (y+1.0)*0.5;
			gl_Position = vec4(x, y, 0, 1);
		})", R"(
		#version 330 core

		layout (location = 0) out vec4 color;
		uniform sampler2D myTexture;
		in vec2 texCoord;
	
		void main()
		{
			color = texture2D(myTexture, texCoord);
			//color = vec4(texCoord.x, texCoord.y, 0, 1);
		}
	)");
}

void FrameBuffer::Resize(int _w, int _h)
{
	if (m_color_buffer)
		delete[] m_color_buffer;
	if (depthBuffer)
		delete[] depthBuffer;
	m_width = _w;
	m_height = _h;
	m_color_buffer = new uint8_t[m_width * m_height * m_bytesPerPixel];
	depthBuffer = new float[m_width * m_height];
}

void FrameBuffer::ClearColor(float red, float green, float blue)
{
	uint8_t r = (uint8_t)(std::clamp<float>(red, 0.f, 1.f) * 255);
	uint8_t g = (uint8_t)(std::clamp<float>(green, 0.f, 1.f) * 255);
	uint8_t b = (uint8_t)(std::clamp<float>(blue, 0.f, 1.f) * 255);

	for (auto i = 0; i < m_height * m_width * m_bytesPerPixel; i += m_bytesPerPixel)
	{
		m_color_buffer[i] = r;
		m_color_buffer[i + 1] = g;
		m_color_buffer[i + 2] = b;
		m_color_buffer[i + 3] = 255;
	}
	for (auto i = 0; i < m_height * m_width; i++)
	{
		depthBuffer[i] = INFINITY;
	}
}

void FrameBuffer::RenderGL()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_color_buffer);

	shader->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	shader->Unbind();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::Save(std::string file_name)
{
	glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_color_buffer);
}

void FrameBuffer::SetPixel(int x, int y, int color, float depth)
{
	//ASSERT(x >= 0 || y < 0 || x >= m_width || y >= m_height, "SetPixel out of range!")
	if (x < 0 || y < 0 || x >= m_width || y >= m_height)
		return;
	int id = y * m_width + x;
	if (depthBuffer[id] < depth) return;
	int idx = m_bytesPerPixel * id;
	m_color_buffer[idx] = RED(color);
	m_color_buffer[idx + 1] = GREEN(color);
	m_color_buffer[idx + 2] = BLUE(color);
	m_color_buffer[idx + 3] = ALPHA(color);
	depthBuffer[id] = depth;
}

int FrameBuffer::GetPixel(int x, int y)
{
	//ASSERT(x < 0 || y < 0 || x >= m_width || y >= m_height, "GetPixel out of range!")
	if (x < 0 || y < 0 || x >= m_width || y >= m_height)
		return 0;
	int idx = m_bytesPerPixel * (y * m_width + x);
	return RGBA(m_color_buffer[idx], m_color_buffer[idx + 1], m_color_buffer[idx + 2], m_color_buffer[idx + 3]);
}

void FrameBuffer::DrawLine(int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;  /* error value e_xy */
	while (true) {
		if (x0 == x1 && y0 == y1) break;
		SetPixel(x0, y0, color, -INFINITY);
		int e2 = 2 * err;
		if (e2 >= dy) {
			err += dy; /* e_xy+e_x > 0 */
			x0 += sx;
		}
		if (e2 <= dx) /* e_xy+e_y < 0 */
		{
			err += dx;
			y0 += sy;
		}
	}
}

void FrameBuffer::DrawRect(int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	for (int x = x0; x != x1; x += sx) {
		for (int y = y0; y != y1; y += sy) {
			SetPixel(x, y, color, 0);
		}
	}
}

void FrameBuffer::DrawTriangles(std::vector<glm::vec3> vertices,
	std::vector<glm::uvec3> triangles, int color)
{
	for (int i = 0; i < triangles.size(); i++)
	{
		glm::vec3 normal = glm::normalize(glm::cross(
			vertices[triangles[i].y] - vertices[triangles[i].x],
			vertices[triangles[i].z] - vertices[triangles[i].x]));
		if (dot({ 0,0,-1 }, normal) < 0)
		{
			DrawTriangle(
				vertices[triangles[i].x],
				vertices[triangles[i].y],
				vertices[triangles[i].z], color);
		}
	}
}
void FrameBuffer::DrawTriangle(
	const glm::vec3& v1,
	const glm::vec3& v2,
	const glm::vec3& v3, int color)
{
	ScanLine(v1, v2, v3, color);
	DrawLine(
		v1.x,
		v1.y,
		v2.x,
		v2.y, 0xFF00FFFF);
	DrawLine(
		v2.x,
		v2.y,
		v3.x,
		v3.y, 0xFF00FFFF);
	DrawLine(
		v3.x,
		v3.y,
		v1.x,
		v1.y, 0xFF00FFFF);
}
void FrameBuffer::ScanLine(
	glm::vec3 v1,
	glm::vec3 v2,
	glm::vec3 v3, int color)
{
	glm::vec3 buf;


	if (v2.y < v1.y)
	{
		buf = v1;
		v1 = v2;
		v2 = buf;
	}
	if (v3.y < v2.y)
	{
		buf = v3;
		v3 = v2;
		v2 = buf;
		if (v2.y < v1.y)
		{
			buf = v1;
			v1 = v2;
			v2 = buf;
		}
	}
	//point on the edge opposite to the middle point, which will be another vertex
	//of the base of two created triangles
	glm::vec3 v4 = v1 + (v3 - v1) * ((v2.y - v1.y) / (v3.y - v1.y));
	if (v2.x < v4.x)
	{
		ScanLineHorizontalBase(v2, v4, v3, color);
		ScanLineHorizontalBase(v2, v4, v1, color);
	}
	else
	{
		ScanLineHorizontalBase(v4, v2, v3, color);
		ScanLineHorizontalBase(v4, v2, v1, color);
	}
}
void FrameBuffer::ScanLineHorizontalBase(
	const glm::vec3& v1baseLeft,
	const glm::vec3& v2baseRight,
	const glm::vec3& v3peak, int color)
{
	int baseY = v1baseLeft.y;
	int peakY = v3peak.y;
	int yDiff = peakY - baseY;
	float depth1 = v1baseLeft.z;
	float depth2 = v2baseRight.z;
	float depth3 = v3peak.z;
	float minX = v1baseLeft.x;
	float maxX = v2baseRight.x;
	int yInc = glm::sign(peakY - baseY);
	float antitangent1 = (v3peak.x - v1baseLeft.x) / (v3peak.y - v1baseLeft.y) * yInc;
	float antitangent2 = (v3peak.x - v2baseRight.x) / (v3peak.y - v2baseRight.y) * yInc;
	for (int y = baseY; y != peakY; y += yInc)
	{
		float q = (float)(y - baseY) / yDiff;
		float lineDepth1 = depth1 * (1 - q) + depth3 * q;
		float lineDepth2 = depth2 * (1 - q) + depth3 * q;
		int xDiff = maxX - minX;
		for (int x = minX; x < maxX; x++)
		{
			q = (float)(x - (int)minX) / xDiff;
			float depth = lineDepth1 * (1-q) + lineDepth2 * q;
			SetPixel(x, y, color, depth);
		}
		minX += antitangent1;
		maxX += antitangent2;
	}

}
