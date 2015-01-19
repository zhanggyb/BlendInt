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

#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Text::Text (const String& text)
 	: AbstractForm(),
 	  str_len_(0),
 	  vao_(0),
 	  text_(text)
 	{
 		atlas_.Generate(128, 32);

 		FTLibrary ft_lib;
 		FTFace ft_face;

 		ft_lib.Initialize();
 		#ifdef __APPLE__
	 		ft_face.New(ft_lib, "/System/Library/Fonts/HelveticaNeue.dfont");
 		#else
	 		ft_face.New(ft_lib, "/usr/share/fonts/adobe-source-han-sans-cn/SourceHanSansCN-Regular.otf");
 		#endif
 		ft_face.SetCharSize(16 << 6, 0, 96, 0);
 		FT_GlyphSlot g = NULL;

 		std::vector<GLfloat> verts(5 * 4 * 4, 0.f);
 		int p [2 * 5];
 		float advance = 0.f;

 		atlas_.bind();
 		for(int i = 0; i < 5; i++) {

 	 		ft_face.LoadChar('A' + i, FT_LOAD_RENDER);
 	 		g = ft_face.face()->glyph;

 	 		atlas_.Upload(g->bitmap.width, g->bitmap.rows, g->bitmap.buffer, &p[i * 2 + 0], &p[i * 2 + 1]);

 	 		verts[i * 16 + 0] = advance + g->bitmap_left;
 	 		verts[i * 16 + 1] = g->bitmap_top - g->bitmap.rows;
 	 		verts[i * 16 + 2] = p[i * 2 + 0];
 	 		verts[i * 16 + 3] = p[i * 2 + 1] + g->bitmap.rows;

 	 		verts[i * 16 + 4] = advance + g->bitmap_left + g->bitmap.width;
 	 		verts[i * 16 + 5] = g->bitmap_top - g->bitmap.rows;
 	 		verts[i * 16 + 6] = p[i * 2 + 0] + g->bitmap.width;
 	 		verts[i * 16 + 7] = p[i * 2 + 1] + g->bitmap.rows;

 	 		verts[i * 16 + 8] = advance + g->bitmap_left;
 	 		verts[i * 16 + 9] = g->bitmap_top;
 	 		verts[i * 16 + 10] = p[i * 2 + 0];
 	 		verts[i * 16 + 11] = p[i * 2 + 1];

 	 		verts[i * 16 + 12] = advance + g->bitmap_left + g->bitmap.width;
 	 		verts[i * 16 + 13] = g->bitmap_top;
 	 		verts[i * 16 + 14] = p[i * 2 + 0] + g->bitmap.width;
 	 		verts[i * 16 + 15] = p[i * 2 + 1];

 	 		advance = advance + (g->advance.x >> 6);
 		}
 		atlas_.reset();

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

		atlas_.bind();

		glUniform2f(Context::shaders->location(Shaders::WIDGET_TEXT_POSITION), x, y);
		glUniform4fv(Context::shaders->location(Shaders::WIDGET_TEXT_COLOR), 1, color.data());
		glUniform1i(Context::shaders->location(Shaders::WIDGET_TEXT_TEXTURE), 0);

		glBindVertexArray(vao_);

		for(int i = 0; i < 5; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		}

		glBindVertexArray(0);

		atlas_.reset();
		GLSLProgram::reset();
 	}

 }
