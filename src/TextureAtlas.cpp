/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <string.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include <BlendInt/TextureAtlas.hpp>
#include <BlendInt/Freetype.hpp>

// Maximum texture width
#define MAXWIDTH 2048

namespace BlendInt {

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


	TextureAtlas::TextureAtlas ()
			: texture_(0), uniform_tex_(-1), attribute_coord_(-1),
			  uniform_color_(-1), vbo_(0),
			  width_(0), height_(0), starting_charcode_(0), stride_(0),
			  glyph_array_(0)
	{
	}

	TextureAtlas::~TextureAtlas ()
	{
		if(glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}

		if(glIsBuffer(vbo_)) {
			glDeleteBuffers(1, &vbo_);
		}

		if(glyph_array_) {
			delete [] glyph_array_;
		}
	}

	void TextureAtlas::initialize()
	{
		program_.attachShaderPair(vs_shader, fs_shader);
		if(!program_.isValid()) {
			std::cerr << "Cannot compile shaders" << std::endl;
			return;
		}
		if(!program_.link()) {
			std::cerr << "Cannot link program" << std::endl;
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

	void TextureAtlas::generate (Freetype* freetype, wchar_t start, int size)
	{
		if(!freetype->valid()) return;

		if(size <= 0) return;

		stride_ = size;
		starting_charcode_ = start;

		if(glyph_array_) {
			delete [] glyph_array_;
			glyph_array_ = 0;
		}
		glyph_array_ = new Glyph[stride_];

		if(glIsTexture(texture_)) {
				glDeleteTextures(1, &texture_);
		}

		FT_GlyphSlot g = freetype->getFontFace()->glyph;

		int roww = 0;
		int rowh = 0;
		 width_ = 0;
		 height_ = 0;

		/* Find minimum size for a texture holding all visible ASCII characters */
		for (wchar_t i = starting_charcode_; i < (starting_charcode_ + stride_ - 1); i++) {
			if (!freetype->loadCharacter(i, FT_LOAD_RENDER)) {
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

		for (wchar_t i = starting_charcode_; i < (starting_charcode_ + stride_ - 1); i++) {
			if (!freetype->loadCharacter(i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
				oy += rowh;
				rowh = 0;
				ox = 0;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			int offset = i - starting_charcode_;
			(glyph_array_ + offset )->advance_x = g->advance.x >> 6;
			(glyph_array_ + offset)->advance_y = g->advance.y >> 6;

			(glyph_array_ + offset)->bitmap_width = g->bitmap.width;
			(glyph_array_ + offset)->bitmap_height = g->bitmap.rows;

			(glyph_array_ + offset)->bitmap_left = g->bitmap_left;
			(glyph_array_ + offset)->bitmap_top = g->bitmap_top;

			(glyph_array_ + offset)->texture_offset_x = ox / (float)width_;
			(glyph_array_ + offset)->texture_offset_y = oy / (float)height_;

			(glyph_array_ + offset)->vertexes[0].x = (glyph_array_ + offset)->bitmap_left;
			(glyph_array_ + offset)->vertexes[0].y = (glyph_array_ + offset)->bitmap_top;
			(glyph_array_ + offset)->vertexes[0].s = (glyph_array_ + offset)->texture_offset_x;
			(glyph_array_ + offset)->vertexes[0].t = (glyph_array_ + offset)->texture_offset_y;

			(glyph_array_ + offset)->vertexes[1].x = (glyph_array_ + offset)->bitmap_left + (glyph_array_ + offset)->bitmap_width;
			(glyph_array_ + offset)->vertexes[1].y = (glyph_array_ + offset)->bitmap_top;
			(glyph_array_ + offset)->vertexes[1].s = (glyph_array_ + offset)->texture_offset_x + (glyph_array_ + offset)->bitmap_width / width_;
			(glyph_array_ + offset)->vertexes[1].t = (glyph_array_ + offset)->texture_offset_y;

			(glyph_array_ + offset)->vertexes[2].x = (glyph_array_ + offset)->bitmap_left;
			(glyph_array_ + offset)->vertexes[2].y = (glyph_array_ + offset)->bitmap_top - (glyph_array_ + offset)->bitmap_height;
			(glyph_array_ + offset)->vertexes[2].s = (glyph_array_ + offset)->texture_offset_x;
			(glyph_array_ + offset)->vertexes[2].t = (glyph_array_ + offset)->texture_offset_y + (glyph_array_ + offset)->bitmap_height / height_;

			(glyph_array_ + offset)->vertexes[3].x = (glyph_array_ + offset)->bitmap_left + (glyph_array_ + offset)->bitmap_width;
			(glyph_array_ + offset)->vertexes[3].y = (glyph_array_ + offset)->bitmap_top;
			(glyph_array_ + offset)->vertexes[3].s = (glyph_array_ + offset)->texture_offset_x + (glyph_array_ + offset)->bitmap_width / width_;
			(glyph_array_ + offset)->vertexes[3].t = (glyph_array_ + offset)->texture_offset_y;

			(glyph_array_ + offset)->vertexes[4].x = (glyph_array_ + offset)->bitmap_left;
			(glyph_array_ + offset)->vertexes[4].y = (glyph_array_ + offset)->bitmap_top - (glyph_array_ + offset)->bitmap_height;
			(glyph_array_ + offset)->vertexes[4].s = (glyph_array_ + offset)->texture_offset_x;
			(glyph_array_ + offset)->vertexes[4].t = (glyph_array_ + offset)->texture_offset_y + (glyph_array_ + offset)->bitmap_height / height_;

			(glyph_array_ + offset)->vertexes[5].x = (glyph_array_ + offset)->bitmap_left + (glyph_array_ + offset)->bitmap_width;
			(glyph_array_ + offset)->vertexes[5].y = (glyph_array_ + offset)->bitmap_top - (glyph_array_ + offset)->bitmap_height;
			(glyph_array_ + offset)->vertexes[5].s = (glyph_array_ + offset)->texture_offset_x + (glyph_array_ + offset)->bitmap_width / width_;
			(glyph_array_ + offset)->vertexes[5].t = (glyph_array_ + offset)->texture_offset_y + (glyph_array_ + offset)->bitmap_height / height_;

			rowh = std::max(rowh, g->bitmap.rows);
			ox += g->bitmap.width + 1;
		}

#ifdef DEBUG
		fprintf(stdout, "Generated %u characters with a %u x %u (%u kb) texture atlas: ",
				stride_, width_, height_, width_ * height_ / 1024);

		std::cerr << ConvertFromString(freetype->font().family) << " with size: "<< freetype->font().size << " and dpi: " << freetype->dpi() << std::endl;
#endif
	}

	bool TextureAtlas::contains(wchar_t charcode)
	{
		int offset = charcode - starting_charcode_;

		if (offset < 0 || offset > (stride_ - 1))  return false;

		return true;
	}

	const Glyph& TextureAtlas::glyph (wchar_t charcode) const
	{
		int offset = charcode - starting_charcode_;
		if(offset < 0 || offset >= stride_) {
			throw std::out_of_range("Charcode not in atlas\n");
		}
		if(!glyph_array_) {
			throw std::runtime_error("No glyph allocated in this atlas\n");
		}

		return *(glyph_array_ + offset);
	}

	/**
	 * Render text using the currently loaded font and currently set font size.
	 * Rendering starts at coordinates (x, y), z is always 0.
	 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
	 */
	void TextureAtlas::render_text(const wchar_t* text, float x, float y, float sx, float sy)
	{
		const wchar_t* p;

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

		Vertex* coords = new Vertex[6 * wcslen(text)];
		int c = 0;

		/* Loop through all characters */
		for (p = text; *p; p++)
		{
			if((*p) < starting_charcode_) continue;
			if((*p) > (starting_charcode_ + stride_ - 1)) continue;

			int offset = *p - starting_charcode_;

			/* Calculate the vertex and texture coordinates */
			float x2 = x + (glyph_array_ + offset)->bitmap_left * sx;
			float y2 = y + (glyph_array_ + offset)->bitmap_top * sy;
			float w = (glyph_array_ + offset)->bitmap_width * sx;
			float h = (glyph_array_ + offset)->bitmap_height * sy;

			/* Advance the cursor to the start of the next character */
			x += (glyph_array_ + offset)->advance_x * sx;
			y += (glyph_array_ + offset)->advance_y * sy;

			/* Skip glyphs that have no pixels */
			if (!w || !h)
				continue;

			(coords + c)->x = x2;
			(coords + c)->y = y2;
			(coords + c)->s = (glyph_array_ + offset)->texture_offset_x;
			(coords + c)->t = (glyph_array_ + offset)->texture_offset_y;
			c++;

			(coords + c)->x = x2 + w;
			(coords + c)->y = y2;
			(coords + c)->s = (glyph_array_ + offset)->texture_offset_x + (glyph_array_ + offset)->bitmap_width / width_;
			(coords + c)->t = (glyph_array_ + offset)->texture_offset_y;
			c++;

			(coords + c)->x = x2;
			(coords + c)->y = y2 - h;
			(coords + c)->s = (glyph_array_ + offset)->texture_offset_x;
			(coords + c)->t = (glyph_array_ + offset)->texture_offset_y + (glyph_array_ + offset)->bitmap_height / height_;
			c++;

			(coords + c)->x = x2 + w;
			(coords + c)->y = y2;
			(coords + c)->s = (glyph_array_ + offset)->texture_offset_x + (glyph_array_ + offset)->bitmap_width / width_;
			(coords + c)->t = (glyph_array_ + offset)->texture_offset_y;
			c++;

			(coords + c)->x = x2;
			(coords + c)->y = y2 - h;
			(coords + c)->s = (glyph_array_ + offset)->texture_offset_x;
			(coords + c)->t = (glyph_array_ + offset)->texture_offset_y + (glyph_array_ + offset)->bitmap_height / height_;
			c++;

			(coords + c)->x = x2 + w;
			(coords + c)->y = y2 - h;
			(coords + c)->s = (glyph_array_ + offset)->texture_offset_x + (glyph_array_ + offset)->bitmap_width / width_;
			(coords + c)->t = (glyph_array_ + offset)->texture_offset_y + (glyph_array_ + offset)->bitmap_height / height_;
			c++;
		}

		/* Draw all the character on the screen in one go */
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6 * wcslen(text), coords, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, c);

		glDisableVertexAttribArray(attribute_coord_);
		delete [] coords;
		coords = 0;

		glUseProgram(0);

		glDisable(GL_BLEND);
	}

}
