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
	if (depthBuffering && depthBuffer[id] < depth) return;
	int idx = m_bytesPerPixel * id;
	m_color_buffer[idx] = RED(color);
	m_color_buffer[idx + 1] = GREEN(color);
	m_color_buffer[idx + 2] = BLUE(color);
	m_color_buffer[idx + 3] = ALPHA(color);
	depthBuffer[id] = depth;
}
void FrameBuffer::SetPixelWithAlpha(int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= m_width || y >= m_height)
		return;
	if (ALPHA(color) == 0) return;
	float q = ALPHA(color) / 255.0f;
	color = RGB((int)(RED(color) * q), (int)(GREEN(color) * q), (int)(BLUE(color) * q));
	int id = y * m_width + x;
	int idx = m_bytesPerPixel * id;
	m_color_buffer[idx] = m_color_buffer[idx] * (1 - q) + RED(color);
	m_color_buffer[idx + 1] = m_color_buffer[idx + 1] * (1 - q) + GREEN(color);
	m_color_buffer[idx + 2] = m_color_buffer[idx + 2] * (1 - q) + BLUE(color);
}

void FrameBuffer::DrawPixmap(int startX, int startY, int width, int height, unsigned char* buffer, int color)
{

	
	int mapStartX = 0;
	if (startX < 0)
	{
		mapStartX -= startX;
		startX = 0;
	}
	int mapStartY = 0;
	if (startY >= m_height)
	{
		mapStartY += startY - (m_width - 1);
		startY = m_width-1;
	}
	for (int fbY = startY, mapY = 0; mapY < height && fbY >= 0; mapY++, fbY--)
	{
		for (int fbX = startX, mapX = mapStartX; mapX < width && fbX < m_width; fbX++, mapX++)
		{
			int id = fbY * m_width + fbX;
			int idx = m_bytesPerPixel * id;
			if (buffer[mapY * width + mapX] != 0)
			{
				int newCol = RGBA(RED(color), GREEN(color), BLUE(color), buffer[mapY * width + mapX]);
				SetPixelWithAlpha(fbX, fbY, newCol);
			}
		}
	}
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
	if (ALPHA(color) == 0) return;
	
	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	for (int x = x0; x != x1; x += sx) {
		for (int y = y0; y != y1; y += sy) {
			SetPixelWithAlpha(x, y, color);
		}
	}
}

int FrameBuffer::getWidth()
{
	return this->m_width;
}
int FrameBuffer::getHeight()
{
	return this->m_height;
}

void FrameBuffer::toggleDepthBuffering()
{
	depthBuffering = !depthBuffering;
}


int floatToIntColor(const glm::vec4& floatColor)
{
	uint8_t r = (uint8_t)(std::clamp<float>(floatColor.r, 0.f, 1.f) * 255);
	uint8_t g = (uint8_t)(std::clamp<float>(floatColor.g, 0.f, 1.f) * 255);
	uint8_t b = (uint8_t)(std::clamp<float>(floatColor.b, 0.f, 1.f) * 255);
	uint8_t a = (uint8_t)(std::clamp<float>(floatColor.a, 0.f, 1.f) * 255);
	return RGBA(r, g, b, a);
}


