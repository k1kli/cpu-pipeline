#pragma once
#include <stdint.h>
#include "Shader.hpp"


#undef RGB
#define RGB(r, g, b) ( ((r) & 0xff) + (((g) & 0xff)<<8) + ( ( (b) & 0xff) <<16) + (0xff<<24) )
#define RGBA(r, g, b, a) ( (r) + (g<<8) + (b<<16) + (a<<24) )
#define RED(color) ((color) & 0xff)
#define GREEN(color) (((color) >> 8) & 0xff)
#define BLUE(color) (((color) >> 16) & 0xff)
#define ALPHA(color) (((color) >> 24) & 0xff)

class FrameBuffer
{
private:
	int m_width;
	int m_height;
	const int m_bytesPerPixel = 4;

	uint8_t *m_color_buffer = nullptr;
	float* depthBuffer = nullptr;

	unsigned int tex = 0;

	Ref<Shader> shader;

	//void DrawTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, int color);
	//void ScanLineHorizontalBase(
	//	const glm::vec3& v1baseLeft,
	//	const glm::vec3& v2baseRight,
	//	const glm::vec3& v3peak,
	//	int color);
	//void ScanLine(
	//	glm::vec3 v1,
	//	glm::vec3 v2,
	//	glm::vec3 v3, int color);

public:
	FrameBuffer(int _w, int _h);
	~FrameBuffer();

	void InitGL();
	void Resize(int _w, int _h);
	void ClearColor(float red, float green, float blue);
	void RenderGL();
	void Save(std::string file_name);

	void SetPixel(int x, int y, int color, float depth);
	int GetPixel(int x, int y);

	void DrawLine(int x0, int y0, int x1, int y1, int color);
	void DrawRect(int x0, int y0, int x1, int y1, int color);
	int getWidth();
	int getHeight();
	//void DrawTriangles(std::vector<glm::vec3> vertices,
	//	std::vector<glm::uvec3> triangles, int color);

};

int floatToIntColor(const glm::vec4& floatColor);

