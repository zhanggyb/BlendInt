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

#include <BlendInt/OpenGL/TextureBuffer.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	TextureBuffer::TextureBuffer()
	: Object()
	{
		glGenVertexArrays(1, &m_vao);

		m_vbo.reset(new GLArrayBuffer);
		m_texture.reset(new GLTexture2D);
	}

	TextureBuffer::~TextureBuffer()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void TextureBuffer::SetCoord (GLfloat x0, GLfloat y0, GLfloat x1,
	        GLfloat y1)
	{
		glBindVertexArray(m_vao);
		if(m_vbo->id() == 0) {
			m_vbo->generate();
		}

		GLfloat vertices[] = {
				// coord	uv
				x0, y0,		0.f, 0.f,
				x1, y0,		1.f, 0.f,
				x0, y1,		0.f, 1.f,
				x1, y1,		1.f, 1.f
		};

		m_vbo->bind();
		m_vbo->set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		m_vbo->reset();
	}

	void TextureBuffer::Draw (GLfloat x, GLfloat y)
	{
		using Stock::Shaders;

		if(m_texture->texture()) {

			m_texture->bind();
			RefPtr<GLSLProgram> program = Shaders::instance->image_program();
			program->use();
			glUniform3f(Shaders::instance->location(Stock::IMAGE_POSITION), x, y, 0.f);
			glUniform1i(Shaders::instance->location(Stock::IMAGE_TEXTURE), 0);
			glUniform1i(Shaders::instance->location(Stock::IMAGE_GAMMA), 0);

			glBindVertexArray(m_vao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			m_texture->reset();
			program->reset();

		}
	}

}
