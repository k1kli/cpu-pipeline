#include "TextDrawer.h"

TextDrawer::TextDrawer(FrameBuffer & fb):fb(fb)
{

	int error = FT_Init_FreeType(&library);
	if (error) { throw "failed to init freetype"; }
	error = FT_New_Face(library, "data/arial.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		throw "unsupported font formata";
	}
	else if (error)
	{
		throw "failed to open font file";
	}
}

void TextDrawer::DrawTextAt(std::string text, int x, int y)
{
	int error = FT_Set_Char_Size(
		face,
		4 * 64,
		8 * 64,
		fb.getWidth(),
		fb.getHeight()
	);
	for (int i = 0; i < text.size(); i++)
	{
		if (text.at(i) == ' ')
		{
			x += face->glyph->advance.x >> 6;
			continue;
		}
		int glyph_index = FT_Get_Char_Index(face, text.at(i));
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		fb.DrawPixmap(x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
		x += face->glyph->bitmap.width;
	}
	
}
