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
 * @file font_shader.cpp
 * @author Florian Märkl
 */

#ifdef _WIN32

#include <GL/glew.h>

#else

#include <GL/gl.h>

#endif

#include <string.h>
#include <stdio.h>

#include "font_shader.h"


const char *vertex_src = "#version 330\n"
		"\n"
		"in vec2 vertex_attr;\n"
		"in vec2 uv_attr;\n"
		"\n"
		"out vec2 uv_var;\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"	uv_var = uv_attr;\n"
		"	gl_Position = vec4(vertex_attr * 2.0 - vec2(1.0), 0.0, 1.0);\n"
		"}";

const char *fragment_src = "#version 330\n"
		"\n"
		"uniform sampler2D atlas_tex_uni;\n"
		"uniform vec4 color_uni;\n"
		"\n"
		"in vec2 uv_var;\n"
		"\n"
		"out vec4 color_out;\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"	float alpha = texture(atlas_tex_uni, uv_var).r;\n"
		"	color_out = vec4(color_uni.rgb, color_uni.a * alpha);\n"
		"}";




void PrintGLInfoLog_(const char *log_title, GLuint handle, const char *shader_name = 0)
{
	GLchar *string;
	GLint size;

	size = 0;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &size);
	if(size > 1)
	{
		string = new GLchar [size];
		glGetShaderInfoLog(handle, size, &size, string);
		printf("%s info log", log_title);
		if(shader_name != 0)
			printf(" for %s:\n", shader_name);
		printf(":\n");
		printf("-----------\n%s\n-----------\n", string);
		delete [] string;
	}
}

ftDefaultFontShader::ftDefaultFontShader(void)
{
	GLint len;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	len = (GLint)strlen(vertex_src);
	glShaderSource(vertex_shader, 1, &vertex_src, &len);
	glCompileShader(vertex_shader);
	PrintGLInfoLog_("Compile", vertex_shader, "Font Vertex Shader");

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	len = (GLint)strlen(fragment_src);
	glShaderSource(fragment_shader, 1, &fragment_src, &len);
	glCompileShader(fragment_shader);
	PrintGLInfoLog_("Compile", fragment_shader, "Font Fragment Shader");

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glBindAttribLocation(program, vertex_attribute, "vertex_attr");
	glBindAttribLocation(program, uv_attribute, "uv_attr");

	glLinkProgram(program);

	glDetachShader(program, vertex_shader);
	glDeleteShader(vertex_shader);

	glDetachShader(program, fragment_shader);
	glDeleteShader(fragment_shader);

	atlas_tex_uniform = glGetUniformLocation(program, "atlas_tex_uni");
	color_uniform = glGetUniformLocation(program, "color_uni");

	Bind();
	glUniform1i(atlas_tex_uniform, atlas_tex_unit);
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

ftDefaultFontShader::~ftDefaultFontShader(void)
{
	glDeleteProgram(program);
}

void ftDefaultFontShader::Bind(void)
{
	glUseProgram(program);
}

void ftDefaultFontShader::SetFontFace(ftFontFace *face)
{
	glActiveTexture(GL_TEXTURE0 + atlas_tex_unit);
	glBindTexture(GL_TEXTURE_2D, face->GetAtlasTexture());
}

void ftDefaultFontShader::SetColor(float r, float g, float b, float a)
{
	glUniform4f(color_uniform, r, g, b, a);
}