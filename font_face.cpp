/*
 * Copyright (C) 2015 by Florian Märkl
 * http://www.metallic-entertainment.com
 *
 * This file is part of cglft.
 *
 * cglft is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cglft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cglft.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * @file font_face.cpp
 * @author Florian Märkl
 */

#include "font_face.h"

#include <cmath>

using namespace std;

ftFontFace::ftFontFace(FT_Face ft_face, unsigned int size)
{
	glyphs = new map<unsigned int, ftGlyph>();

	FT_Set_Pixel_Sizes(ft_face, 0, size);

	unsigned int max_width = 0;
	unsigned int max_height = 0;
	unsigned int char_count = 0;

	for(unsigned int c=0; c<256; c++)
	{
		FT_UInt char_index = FT_Get_Char_Index(ft_face, c);
		if(!char_index)
			continue;

		if(FT_Load_Glyph(ft_face, char_index, FT_LOAD_RENDER))
			continue;

		if(ft_face->glyph->bitmap.width > max_width)
			max_width = ft_face->glyph->bitmap.width;

		if(ft_face->glyph->bitmap.rows > max_height)
			max_height = ft_face->glyph->bitmap.rows;

		ftGlyph glyph;

		glyph.character = c;
		glyph.width = ft_face->glyph->bitmap.width;
		glyph.height = ft_face->glyph->bitmap.rows;

		char_count++;
	}


	unsigned int columns, rows;

	columns = (unsigned int)ceil(sqrt((double)char_count));
	rows = (unsigned int)ceil((double)char_count / (double)columns);

	atlas_width = columns * max_width;
	atlas_height = rows * max_height;
	unsigned int atlas_size = atlas_width * atlas_height;
	atlas_data = new unsigned char[atlas_size];
	memset(atlas_data, 0, atlas_size);

	std::map<unsigned int, ftGlyph> &glyphs_map = *glyphs;

	unsigned int ci = 0;
	for(unsigned int c=0; c<256; c++)
	{
		FT_UInt char_index = FT_Get_Char_Index(ft_face, c);
		if(!char_index)
			continue;

		if(FT_Load_Glyph(ft_face, char_index, FT_LOAD_RENDER))
			continue;

		ftGlyph &glyph = glyphs_map[c];
		FT_GlyphSlot ft_glyph = ft_face->glyph;

		glyph.atlas_x = (ci % columns) * max_width;
		glyph.atlas_y = (ci / columns) * max_height;
		glyph.width = ft_glyph->bitmap.width;
		glyph.height = ft_glyph->bitmap.rows;
		glyph.left = ft_glyph->bitmap_left;
		glyph.top = ft_glyph->bitmap_top;
		glyph.advance_x = (int)(ft_glyph->advance.x >> 6); // division by 64
		glyph.advance_y = (int)(ft_glyph->advance.y >> 6);

		for (unsigned int y = 0; y < glyph.height; y++)
		{
			unsigned int dst = (glyph.atlas_y + y) * atlas_width + glyph.atlas_x;
			memcpy(atlas_data + dst, ft_glyph->bitmap.buffer + ft_glyph->bitmap.width * y, glyph.width);
		}

		ci++;
	}

	CreateGLTexture();
}

ftFontFace::~ftFontFace(void)
{
	glDeleteTextures(1, &atlas_tex);
	delete [] atlas_data;
}

void ftFontFace::CreateGLTexture(void)
{
	glGenTextures(1, &atlas_tex);
	glBindTexture(GL_TEXTURE_2D, atlas_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

ftGlyph *ftFontFace::GetGlyph(unsigned int character)
{
	map<unsigned int, ftGlyph>::iterator i = glyphs->find(character);

	if(i == glyphs->end())
		return 0;

	return &(i->second);
}