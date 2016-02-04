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
 * @file font_renderer.cpp
 * @author Florian Märkl
 */

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "font_shader.h"
#include "font_renderer.h"

ftFontRenderer::ftFontRenderer(void)
{
	glGenBuffers(1, &vertex_vbo);
	glGenBuffers(1, &uv_vbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glEnableVertexAttribArray(ftFontShader::vertex_attribute);
	glVertexAttribPointer(ftFontShader::vertex_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glEnableVertexAttribArray(ftFontShader::uv_attribute);
	glVertexAttribPointer(ftFontShader::uv_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

ftFontRenderer::~ftFontRenderer(void)
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertex_vbo);
	glDeleteBuffers(1, &uv_vbo);
}

void ftFontRenderer::RenderText(ftFontShader *shader, ftFontFace *face, const char *text, float x, float y, float scale_x, float scale_y, Alignment align)
{
	unsigned int text_len = (unsigned int)strlen(text);

	float *vertex_data = new float[text_len * 12];
	float *uv_data = new float[text_len * 12];

	float atlas_width = face->GetAtlasWidth();
	float atlas_height = face->GetAtlasHeight();

	float pen_x, pen_y;
	pen_x = x * scale_x;
	pen_y = y * scale_y;

	float *vertex_data_cur = vertex_data;
	float *uv_data_cur = uv_data;
	unsigned int render_chars = 0;

	for(unsigned int i=0; i<text_len; i++)
	{
		char c = text[i];
		ftGlyph *glyph = face->GetGlyph((unsigned int)((unsigned char)c));
		if(!glyph)
			continue;

		vertex_data_cur[0] = pen_x + glyph->left * scale_x;							vertex_data_cur[1] = pen_y + glyph->top * scale_y;
		vertex_data_cur[2] = pen_x + glyph->left * scale_x;							vertex_data_cur[3] = pen_y - (glyph->height - glyph->top) * scale_y;
		vertex_data_cur[4] = pen_x + (glyph->left + glyph->width) * scale_x;		vertex_data_cur[5] = pen_y + glyph->top * scale_y;

		vertex_data_cur[6] = vertex_data_cur[4];									vertex_data_cur[7] = vertex_data_cur[5];
		vertex_data_cur[8] = vertex_data_cur[2];									vertex_data_cur[9] = vertex_data_cur[3];
		vertex_data_cur[10] = vertex_data_cur[0] + glyph->width * scale_x;			vertex_data_cur[11] = pen_y - (glyph->height - glyph->top) * scale_y;


		uv_data_cur[0] = (float)glyph->atlas_x / atlas_width; 						uv_data_cur[1] = (float)glyph->atlas_y / atlas_height;
		uv_data_cur[2] = (float)glyph->atlas_x / atlas_width; 						uv_data_cur[3] = (float)(glyph->atlas_y + glyph->height) / atlas_height;
		uv_data_cur[4] = (float)(glyph->atlas_x + glyph->width) / atlas_width; 		uv_data_cur[5] = (float)glyph->atlas_y / atlas_height;

		uv_data_cur[6] = uv_data_cur[4];											uv_data_cur[7] = uv_data_cur[5];
		uv_data_cur[8] = uv_data_cur[2];											uv_data_cur[9] = uv_data_cur[3];
		uv_data_cur[10] = (float)(glyph->atlas_x + glyph->width) / atlas_width; 	uv_data_cur[11] = (float)(glyph->atlas_y + glyph->height) / atlas_height;


		pen_x += glyph->advance_x * scale_x;
		pen_y += glyph->advance_y * scale_y;
		vertex_data_cur += 12;
		uv_data_cur += 12;
		render_chars++;
	}


	if(align == CENTER || align == RIGHT)
	{
		float offset;
		if(align == CENTER)
			offset = (pen_x - x * scale_x) / 2.0f;
		else
			offset = pen_x - x * scale_x;

		for(unsigned int i=0; i<text_len*6; i++)
			vertex_data[i*2] -= offset;
	}



	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * render_chars * 12, vertex_data, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * render_chars * 12, uv_data, GL_DYNAMIC_DRAW);

	delete [] vertex_data;
	delete [] uv_data;



	shader->SetFontFace(face);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, render_chars * 6);
}