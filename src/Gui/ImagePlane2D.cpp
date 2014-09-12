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

#include <BlendInt/Gui/ImagePlane2D.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ImagePlane2D::ImagePlane2D()
	: Object(),
	  vao_(0)
	{
		glGenVertexArrays(1, &vao_);
		texture_.reset(new GLTexture2D);
	}

	ImagePlane2D::~ImagePlane2D()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ImagePlane2D::SetCoord(GLfloat x0, GLfloat y0, GLfloat x1,
			GLfloat y1)
	{
		glBindVertexArray(vao_);
		if(vertex_buffer_.id() == 0) {
			vertex_buffer_.generate();
		}

		GLfloat vertices[] = {
				// coord	uv
				x0, y0,		0.f, 0.f,
				x1, y0,		1.f, 0.f,
				x0, y1,		0.f, 1.f,
				x1, y1,		1.f, 1.f
		};

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::IMAGE_COORD));
		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::IMAGE_UV));
		glVertexAttribPointer (Shaders::instance->location (Stock::IMAGE_COORD),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (Shaders::instance->location (Stock::IMAGE_UV), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		vertex_buffer_.reset();
		glBindVertexArray(0);
	}

	void ImagePlane2D::Draw(GLfloat x, GLfloat y)
	{
		if(texture_->texture()) {

			texture_->bind();
			Shaders::instance->image_program()->use();
			glUniform3f(Shaders::instance->location(Stock::IMAGE_POSITION), x, y, 0.f);
			glUniform1i(Shaders::instance->location(Stock::IMAGE_TEXTURE), 0);
			glUniform1i(Shaders::instance->location(Stock::IMAGE_GAMMA), 0);

			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			texture_->reset();
			GLSLProgram::reset();

		}
	}

}
