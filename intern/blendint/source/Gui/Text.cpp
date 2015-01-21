/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

 #include <BlendInt/Gui/Text.hpp>

#include <BlendInt/Font/FtFace.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Text::Text (const String& text)
 	: AbstractForm(),
 	  ascender_(0),
 	  descender_(0),
 	  vao_(0),
 	  text_(text)
 	{
		int width;
 		std::vector<GLfloat> verts;
        GenerateTextVertices(verts, &width, &ascender_, &descender_);

        set_size(width, ascender_ - descender_);

 		glGenVertexArrays(1, &vao_);
 		glBindVertexArray(vao_);

 		vbo_.generate();
 		vbo_.bind();
 		vbo_.set_data(sizeof(GLfloat) * verts.size(), &verts[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_TEXT_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_TEXT_COORD),
				4, GL_FLOAT, GL_FALSE, 0, 0);

 		glBindVertexArray(0);
 		vbo_.reset();
 	}

	Text::Text (const Text& text)
	: AbstractForm(),
	  ascender_(0),
	  descender_(0),
	  vao_(0),
	  text_(text.text_)
	{
		int width;
 		std::vector<GLfloat> verts;
        GenerateTextVertices(verts, &width, &ascender_, &descender_);

        set_size(width, ascender_ - descender_);

        glGenVertexArrays(1, &vao_);
 		glBindVertexArray(vao_);

 		vbo_.generate();
 		vbo_.bind();
 		vbo_.set_data(sizeof(GLfloat) * verts.size(), &verts[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_TEXT_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_TEXT_COORD),
				4, GL_FLOAT, GL_FALSE, 0, 0);

 		glBindVertexArray(0);
 		vbo_.reset();
	}

 	Text::~Text ()
 	{
 		glDeleteVertexArrays(1, &vao_);
 	}

 	void Text::SetText (const String& text)
 	{
 		text_ = text;

 		int width;
 		std::vector<GLfloat> verts;
        GenerateTextVertices(verts, &width, &ascender_, &descender_);

        vbo_.bind();
 		vbo_.set_data(sizeof(GLfloat) * verts.size(), &verts[0]);
 		vbo_.reset();

 		set_size(width, ascender_ - descender_);
 	}

 	void Text::SetFont(const Font& font)
 	{
 		if(font_ == font) return;

 		font_ = font;

 		int width;
 		std::vector<GLfloat> verts;
        GenerateTextVertices(verts, &width, &ascender_, &descender_);

        vbo_.bind();
 		vbo_.set_data(sizeof(GLfloat) * verts.size(), &verts[0]);
 		vbo_.reset();

 		set_size(width, ascender_ - descender_);
 	}

 	Text& Text::operator = (const Text& orig)
 	{
 		font_ = orig.font_;
 		text_ = orig.text_;

 		int width;
 		std::vector<GLfloat> verts;
        GenerateTextVertices(verts, &width, &ascender_, &descender_);

        vbo_.bind();
 		vbo_.set_data(sizeof(GLfloat) * verts.size(), &verts[0]);
 		vbo_.reset();

        set_size(width, ascender_ - descender_);

 		return *this;
 	}

 	Text& Text::operator = (const String& text)
 	{
 		text_ = text;

 		int width;
 		std::vector<GLfloat> verts;
        GenerateTextVertices(verts, &width, &ascender_, &descender_);

        vbo_.bind();
 		vbo_.set_data(sizeof(GLfloat) * verts.size(), &verts[0]);
 		vbo_.reset();

        set_size(width, ascender_ - descender_);

 		return *this;
 	}

 	void Text::PerformSizeUpdate (const Size& size)
 	{
 		// Do nothing, this form does not allow resize
 	}

 	void Text::Draw (float x, float y, short gamma) const
 	{
 		Color color(0x000000FF);

 		Draw(x, y, color, gamma);
 	}

 	void Text::Draw (float x, float y, const Color& color, short gamma) const
 	{
		Context::shaders->widget_text_program()->use();

		glActiveTexture(GL_TEXTURE0);

		font_.bind_texture();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_TEXT_POSITION), x, y);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_TEXT_COLOR), 1, color.data());
		glUniform1i(Context::shaders->location(Shaders::WIDGET_TEXT_TEXTURE), 0);

		glBindVertexArray(vao_);
		size_t str_len = text_.length();
		for(size_t i = 0; i < str_len; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		}
		glBindVertexArray(0);
		font_.release_texture();

		GLSLProgram::reset();
 	}

 	void Text::Draw (float x, float y, size_t length, size_t start,
 	        short gamma) const
 	{
 		Color color(0x000000FF);
 		Draw(x, y, length, start, color, gamma);
 	}

 	void Text::Draw (float x, float y, size_t length, size_t start,
 	        const Color& color, short gamma) const
 	{
		Context::shaders->widget_text_program()->use();

		glActiveTexture(GL_TEXTURE0);

		font_.bind_texture();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_TEXT_POSITION), x, y);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_TEXT_COLOR), 1, color.data());
		glUniform1i(Context::shaders->location(Shaders::WIDGET_TEXT_TEXTURE), 0);

		glBindVertexArray(vao_);
		size_t str_len = text_.length();
		size_t last = std::min(start + length, str_len);
		for(size_t i = start; i < last; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		}

		glBindVertexArray(0);
		font_.release_texture();

		GLSLProgram::reset();
 	}

	void Text::Draw (float x, float y, float width, short gamma) const
	{
 		Color color(0x000000FF);
 		Draw(x, y, width, color, gamma);
	}

	void Text::Draw (float x, float y, float width, const Color& color,
	        short gamma) const
	{
		if(width <= 0.f) return;

		Context::shaders->widget_text_program()->use();

		glActiveTexture(GL_TEXTURE0);

		font_.bind_texture();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_TEXT_POSITION), x, y);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_TEXT_COLOR), 1, color.data());
		glUniform1i(Context::shaders->location(Shaders::WIDGET_TEXT_TEXTURE), 0);

		glBindVertexArray(vao_);

		const Glyph* g = 0;
		float max = 0.f;
		int count = 0;
		String::const_iterator next_it;
		Kerning kerning;

		for(String::const_iterator it = text_.begin(); it != text_.end(); it++)
		{
			g = font_.glyph(*it);

			if(font_.has_kerning()) {

                next_it = it + 1;
                if(next_it != text_.end()) {
                	kerning = font_.GetKerning(*it, *next_it, Font::KerningDefault);
                	max += (g->advance_x + kerning.x);
                } else {
                	max += g->advance_x;
                }

			} else {
				max += g->advance_x;
			}

			if (max > width) break;

			glDrawArrays(GL_TRIANGLE_STRIP, count * 4, 4);

			count++;
		}

		glBindVertexArray(0);
		font_.release_texture();

		GLSLProgram::reset();
	}

    void Text::GenerateTextVertices(std::vector<GLfloat> &verts, int* ptr_width, int* ptr_ascender, int* ptr_descender)
    {
        size_t buf_size = text_.length() * 4 * 4;
        if(verts.size() != buf_size) {
            verts.resize(buf_size, 0.f);
        }
        
        int w = 0;	// width
        int a = 0;	// ascender
        int d = 0;	// descender
        const Glyph* g = 0;

    	String::const_iterator next_it;

        int count = 0;
    	if(font_.has_kerning()) {

        	Kerning kerning;
        	for(String::const_iterator it = text_.begin(); it != text_.end(); it++)
            {
                g = font_.glyph(*it);

                verts[count * 16 + 0] = w + g->bitmap_left;
                verts[count * 16 + 1] = g->bitmap_top - g->bitmap_height;
                verts[count * 16 + 2] = g->offset_u;
                verts[count * 16 + 3] = g->offset_v + g->bitmap_height;

                verts[count * 16 + 4] = w + g->bitmap_left + g->bitmap_width;
                verts[count * 16 + 5] = g->bitmap_top - g->bitmap_height;
                verts[count * 16 + 6] = g->offset_u + g->bitmap_width;
                verts[count * 16 + 7] = g->offset_v + g->bitmap_height;

                verts[count * 16 + 8] = w + g->bitmap_left;
                verts[count * 16 + 9] = g->bitmap_top;
                verts[count * 16 + 10] = g->offset_u;
                verts[count * 16 + 11] = g->offset_v;

                verts[count * 16 + 12] = w + g->bitmap_left + g->bitmap_width;
                verts[count * 16 + 13] = g->bitmap_top;
                verts[count * 16 + 14] = g->offset_u + g->bitmap_width;
                verts[count * 16 + 15] = g->offset_v;

                next_it = it + 1;
                if(next_it != text_.end()) {
                	kerning = font_.GetKerning(*it, *next_it, Font::KerningDefault);
                    w += (g->advance_x + kerning.x);
                } else {
                	w += g->advance_x;
                }
                a = std::max(g->bitmap_top, a);
                d = std::min(g->bitmap_top - g->bitmap_height, d);

                count++;
            }

    	} else {

        	for(String::const_iterator it = text_.begin(); it != text_.end(); it++)
            {
                g = font_.glyph(*it);

                verts[count * 16 + 0] = w + g->bitmap_left;
                verts[count * 16 + 1] = g->bitmap_top - g->bitmap_height;
                verts[count * 16 + 2] = g->offset_u;
                verts[count * 16 + 3] = g->offset_v + g->bitmap_height;

                verts[count * 16 + 4] = w + g->bitmap_left + g->bitmap_width;
                verts[count * 16 + 5] = g->bitmap_top - g->bitmap_height;
                verts[count * 16 + 6] = g->offset_u + g->bitmap_width;
                verts[count * 16 + 7] = g->offset_v + g->bitmap_height;

                verts[count * 16 + 8] = w + g->bitmap_left;
                verts[count * 16 + 9] = g->bitmap_top;
                verts[count * 16 + 10] = g->offset_u;
                verts[count * 16 + 11] = g->offset_v;

                verts[count * 16 + 12] = w + g->bitmap_left + g->bitmap_width;
                verts[count * 16 + 13] = g->bitmap_top;
                verts[count * 16 + 14] = g->offset_u + g->bitmap_width;
                verts[count * 16 + 15] = g->offset_v;

                w += (g->advance_x);
                a = std::max(g->bitmap_top, a);
                d = std::min(g->bitmap_top - g->bitmap_height, d);

                count++;
            }

    	}

    	if(ptr_width) *ptr_width = w;
    	if(ptr_ascender) *ptr_ascender = a;
    	if(ptr_descender) *ptr_descender = d;
    }
    
 }

