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
 * @file font_renderer.h
 * @author Florian Märkl
 */

#ifndef CGLFT_FONT_RENDERER_H
#define CGLFT_FONT_RENDERER_H

#include "cglft_export.h"

#ifndef CGLFT_DONT_INCLUDE_GL
#ifdef _WIN32

#include <GL/glew.h>

#else

#include <GL/gl.h>

#endif
#endif

#include "font_face.h"

class CGLFT_EXPORT ftFontRenderer
{
	private:
		ftFontShader *shader;

		GLuint vertex_vbo;
		GLuint uv_vbo;
		GLuint vao;

	public:
		enum Alignment { LEFT, CENTER, RIGHT };

		ftFontRenderer(void);
		~ftFontRenderer(void);

		void RenderText(ftFontShader *shader, ftFontFace *face, const char *text, float x = 0.0f, float y = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f, Alignment align = LEFT);
};


#endif
