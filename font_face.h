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
 * @file font_face.h
 * @author Florian Märkl
 */

#ifndef CGLFT_FONT_FACE_H
#define CGLFT_FONT_FACE_H

#include "cglft_export.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef _WIN32

#include <GL/glew.h>

#else

#include <GL/gl.h>

#endif


#include <map>

#include "glyph.h"


class CGLFT_EXPORT ftFontFace
{
	private:
		std::map<unsigned int, ftGlyph> *glyphs;

		unsigned int atlas_width, atlas_height;
		unsigned char *atlas_data;
		GLuint atlas_tex;

		void CreateGLTexture(void);

	public:
		ftFontFace(FT_Face ft_face, unsigned int size);
		~ftFontFace(void);

		GLuint GetAtlasTexture(void)		{ return atlas_tex; }
		ftGlyph *GetGlyph(unsigned int character);

		unsigned int GetAtlasWidth(void)	{ return atlas_width; }
		unsigned int GetAtlasHeight(void)	{ return atlas_height; }
};

#endif
