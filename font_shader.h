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
 * @file font_shader.h
 * @author Florian Märkl
 */

#ifndef CGLFT_FONT_SHADER_H
#define CGLFT_FONT_SHADER_H

#include "cglft_export.h"

#ifndef CGLFT_DONT_INCLUDE_GL
#ifdef _WIN32

#include <GL/glew.h>

#else

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#endif
#endif

#include "font_face.h"

class CGLFT_EXPORT ftFontShader
{
	public:
		static const int vertex_attribute = 0;
		static const int uv_attribute = 1;

		virtual ~ftFontShader(void) {}
		virtual void SetFontFace(ftFontFace *face) =0;
};

class CGLFT_EXPORT ftDefaultFontShader : public ftFontShader
{
	private:
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		GLint atlas_tex_uniform;
		GLint color_uniform;

		static const int atlas_tex_unit = 0;
	public:

		ftDefaultFontShader(void);
		~ftDefaultFontShader(void);

		void Bind(void);

		void SetFontFace(ftFontFace *face);
		void SetColor(float r, float g, float b, float a = 1.0f);
};

#endif
