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
 * @file glyph.h
 * @author Florian Märkl
 */

#ifndef CGLFT_GLYPH_H
#define CGLFT_GLYPH_H

struct ftGlyph
{
	unsigned int character;

	unsigned int atlas_x;
	unsigned int atlas_y;

	unsigned int width;
	unsigned int height;

	int left;
	int top;

	int advance_x;
	int advance_y;
};

#endif
