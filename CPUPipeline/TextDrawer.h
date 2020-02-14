#pragma once
#include <FrameBuffer.h>
#include <ft2build.h>
#include FT_FREETYPE_H
class TextDrawer
{
public:
	TextDrawer(FrameBuffer& fb);
	void DrawTextAt(std::string text, int x, int y, int color, unsigned int charHeight);
	int GetTextWidth(std::string text, unsigned int charHeight);
private:
	FrameBuffer& fb;
	FT_Library library;
	FT_Face face;
};

