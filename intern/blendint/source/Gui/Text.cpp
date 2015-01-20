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
 	  str_len_(0),
 	  vao_(0),
 	  text_(text)
 	{
 		std::vector<GLfloat> verts(5 * 4 * 4, 0.f);

 		float advance = 0.f;

 		const GlyphMetrics* g;

 		int i = 0;
 		for(String::const_iterator it = text.begin(); it != text.end(); it++)
 		{
 			g = font_.glyph(*it);

 	 		verts[i * 16 + 0] = advance + g->bitmap_left;
 	 		verts[i * 16 + 1] = g->bitmap_top - g->bitmap_height;
 	 		verts[i * 16 + 2] = g->offset_u;
 	 		verts[i * 16 + 3] = g->offset_v + g->bitmap_height;

 	 		verts[i * 16 + 4] = advance + g->bitmap_left + g->bitmap_width;
 	 		verts[i * 16 + 5] = g->bitmap_top - g->bitmap_height;
 	 		verts[i * 16 + 6] = g->offset_u + g->bitmap_width;
 	 		verts[i * 16 + 7] = g->offset_v + g->bitmap_height;

 	 		verts[i * 16 + 8] = advance + g->bitmap_left;
 	 		verts[i * 16 + 9] = g->bitmap_top;
 	 		verts[i * 16 + 10] = g->offset_u;
 	 		verts[i * 16 + 11] = g->offset_v;

 	 		verts[i * 16 + 12] = advance + g->bitmap_left + g->bitmap_width;
 	 		verts[i * 16 + 13] = g->bitmap_top;
 	 		verts[i * 16 + 14] = g->offset_u + g->bitmap_width;
 	 		verts[i * 16 + 15] = g->offset_v;

 	 		advance = advance + g->advance_x;

 	 		i++;
 		}

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

 	}

 	void Text::PerformSizeUpdate (const Size& size)
 	{

 	}

 	void Text::Draw (float x, float y, short gamma) const
 	{
 		Color color(0x000000FF);

		Context::shaders->widget_text_program()->use();

		glActiveTexture(GL_TEXTURE0);

		font_.bind();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_TEXT_POSITION), x, y);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_TEXT_COLOR), 1, color.data());
		glUniform1i(Context::shaders->location(Shaders::WIDGET_TEXT_TEXTURE), 0);

		glBindVertexArray(vao_);

		for(int i = 0; i < 5; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		}

		glBindVertexArray(0);

		font_.reset();

		GLSLProgram::reset();
 	}

 }
