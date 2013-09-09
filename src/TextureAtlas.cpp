/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warrantexture_coord_offset_y of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <string.h>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include <BIL/TextureAtlas.hpp>
#include <BIL/Freetype.hpp>

// Maximum texture width
#define MAXWIDTH 1024

namespace BIL {

	const char* TextureAtlas::vs_shader =
			"attribute vec4 coord;"
			"varying vec2 texpos;"
			""
			"void main(void) {"
			"  gl_Position = gl_ModelViewProjectionMatrix * vec4(coord.xy, 0, 1);"
			"  texpos = coord.zw;"
			"}";

	const char* TextureAtlas::fs_shader =
			"varying vec2 texpos;"
			"uniform sampler2D tex;"
			"uniform vec4 color;"
			""
			"void main(void) {"
			"  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;"
			"}";


	TextureAtlas::TextureAtlas (const std::string& filename)
			: texture_(0), uniform_tex_(-1), attribute_coord_(-1),
			  uniform_color_(-1), vbo_(0),
			  width_(0), height_(0), filename_(filename), font_size_(12)
	{
		 memset(c_, 0, sizeof c_);
	}

	TextureAtlas::~TextureAtlas ()
	{
		if(glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}

		if(glIsBuffer(vbo_)) {
			glDeleteBuffers(1, &vbo_);
		}
	}

	void TextureAtlas::initialize()
	{
		program_.attachShaderPair(vs_shader, fs_shader);
		program_.link();
		if(!program_.isValid()) {
			std::cerr << "Cannot compile shaders" << std::endl;
			return;
		}

		attribute_coord_ = program_.getAttributeLocation("coord");
		uniform_tex_ = program_.getUniformLocation("tex");
		uniform_color_ = program_.getUniformLocation("color");

		if(attribute_coord_ == -1 || uniform_tex_ == -1 || uniform_color_ == -1) {
			std::cerr << "Fatal Error: cannot get attributes and uniforms" << std::endl;
			exit(-1);
		}

		// Create the vertex buffer object
		glGenBuffers(1, &vbo_);
	}

	void TextureAtlas::generate ()
	{
		Freetype freetype;
		freetype.open(filename_, font_size_, 96);
		if(!freetype.valid()) return;

		freetype.setPixelSize(0, font_size_);

		FT_GlyphSlot g = freetype.getFontFace()->glyph;

		int roww = 0;
		int rowh = 0;
		 width_ = 0;
		 height_ = 0;


		/* Find minimum size for a texture holding all visible ASCII characters */
		for (unsigned int i = 32; i < 128; i++) {
			if (!freetype.loadCharacter(static_cast<unsigned long>(i), FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}
			if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
				width_ = std::max(width_, static_cast<unsigned int>(roww));
				height_ += rowh;
				roww = 0;
				rowh = 0;
			}
			roww += g->bitmap.width + 1;
			rowh = std::max(rowh, g->bitmap.rows);
		}

		width_ = std::max(width_, static_cast<unsigned int>(roww));
		height_ += rowh;

		/* Create a texture that will be used to hold all ASCII glyphs */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glUniform1i(uniform_tex_, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width_, height_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Paste all glyph bitmaps into the texture, remembering the offset */
		int ox = 0;
		int oy = 0;

		rowh = 0;

		for (int i = 32; i < 128; i++) {
			if (!freetype.loadCharacter(i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
				oy += rowh;
				rowh = 0;
				ox = 0;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			c_[i].advance_x = g->advance.x >> 6;
			c_[i].advance_y = g->advance.y >> 6;

			c_[i].bitmap_width = g->bitmap.width;
			c_[i].bitmap_height = g->bitmap.rows;

			c_[i].bitmap_left = g->bitmap_left;
			c_[i].bitmap_top = g->bitmap_top;

			c_[i].texture_coord_offset_x = ox / (float)width_;
			c_[i].texture_coord_offset_y = oy / (float)height_;

			rowh = std::max(rowh, g->bitmap.rows);
			ox += g->bitmap.width + 1;
		}

		fprintf(stderr, "Generated a %u x %u (%u kb) texture atlas\n", width_, height_, width_ * height_ / 1024);
		freetype.close();
	}

	/**
	 * Render text using the currently loaded font and currently set font size.
	 * Rendering starts at coordinates (x, y), z is always 0.
	 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
	 */
	void TextureAtlas::render_text(const char *text, float x, float y, float sx, float sy)
	{
		const uint8_t *p;

		glUseProgram(program_.id());

		/* White background */
		//glClearColor(1, 1, 1, 1);
		//glClear(GL_COLOR_BUFFER_BIT);

		/* Enable blending, necessary for our alpha texture */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLfloat black[4] = { 0, 0, 0, 1 };
		//GLfloat red[4] = { 1, 0, 0, 1 };
		//GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

		/* Set color to black */
		glUniform4fv(uniform_color_, 1, black);

		/* Use the texture containing the atlas */
		glBindTexture(GL_TEXTURE_2D, texture_);
		glUniform1i(uniform_tex_, 0);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(attribute_coord_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glVertexAttribPointer(attribute_coord_, 4, GL_FLOAT, GL_FALSE, 0, 0);

		point* coords = new point[6 * strlen(text)];
		int c = 0;

		/* Loop through all characters */
		for (p = (const uint8_t *)text; *p; p++) {
			/* Calculate the vertex and texture coordinates */
			float x2 = x + c_[*p].bitmap_left * sx;
			//float x2 = x + c_[*p].bitmap_left;

			float y2 = -y - c_[*p].bitmap_top * sy;
			//float y2 = -y - c_[*p].bitmap_top;

			float w = c_[*p].bitmap_width * sx;
			//float w = c_[*p].bitmap_width;

			float h = c_[*p].bitmap_height * sy;
			//float h = c_[*p].bitmap_height;

			/* Advance the cursor to the start of the next character */
			x += c_[*p].advance_x * sx;
			//x += c_[*p].advance_x;
			y += c_[*p].advance_y * sy;
			//y += c_[*p].advance_y;

			/* Skip glyphs that have no pixels */
			if (!w || !h)
				continue;

			(coords + c)->x = x2;
			(coords + c)->y = -y2;
			(coords + c)->s = c_[*p].texture_coord_offset_x;
			(coords + c)->t = c_[*p].texture_coord_offset_y;
			c++;

			(coords + c)->x = x2 + w;
			(coords + c)->y = -y2;
			(coords + c)->s = c_[*p].texture_coord_offset_x + c_[*p].bitmap_width / width_;
			(coords + c)->t = c_[*p].texture_coord_offset_y;
			c++;

			(coords + c)->x = x2;
			(coords + c)->y = -y2 - h;
			(coords + c)->s = c_[*p].texture_coord_offset_x;
			(coords + c)->t = c_[*p].texture_coord_offset_y + c_[*p].bitmap_height / height_;
			c++;

			(coords + c)->x = x2 + w;
			(coords + c)->y = -y2;
			(coords + c)->s = c_[*p].texture_coord_offset_x + c_[*p].bitmap_width / width_;
			(coords + c)->t = c_[*p].texture_coord_offset_y;
			c++;

			(coords + c)->x = x2;
			(coords + c)->y = -y2 - h;
			(coords + c)->s = c_[*p].texture_coord_offset_x;
			(coords + c)->t = c_[*p].texture_coord_offset_y + c_[*p].bitmap_height / height_;
			c++;

			(coords + c)->x = x2 + w;
			(coords + c)->y = -y2 - h;
			(coords + c)->s = c_[*p].texture_coord_offset_x + c_[*p].bitmap_width / width_;
			(coords + c)->t = c_[*p].texture_coord_offset_y + c_[*p].bitmap_height / height_;
			c++;
		}

		/* Draw all the character on the screen in one go */
		glBufferData(GL_ARRAY_BUFFER, sizeof(point) * 6 * strlen(text), coords, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, c);

		glDisableVertexAttribArray(attribute_coord_);
		delete [] coords;

		glUseProgram(0);

		glDisable(GL_BLEND);
	}

}
