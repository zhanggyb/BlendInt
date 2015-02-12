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

#include <gui/image-plane.hpp>

#include <core/types.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	ImagePlane2D::ImagePlane2D()
	: Object(),
	  vao_(0)
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		vertex_buffer_.generate();

		GLfloat vertices[] = {
				// coord	uv
				0.f, 0.f,		0.f, 0.f,
				200.f, 0.f,		1.f, 0.f,
				0.f, 200.f,		0.f, 1.f,
				200.f, 200.f,		1.f, 1.f
		};

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				AttributeCoord);
		glEnableVertexAttribArray (
				AttributeUV);
		glVertexAttribPointer (AttributeCoord,
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (AttributeUV, 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vertex_buffer_.reset();

		texture_.reset(new GLTexture2D);
	}

	ImagePlane2D::~ImagePlane2D()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ImagePlane2D::SetCoord(GLfloat x0, GLfloat y0, GLfloat x1,
			GLfloat y1)
	{
		GLfloat vertices[] = {
				// coord	uv
				x0, y0,		0.f, 0.f,
				x1, y0,		1.f, 0.f,
				x0, y1,		0.f, 1.f,
				x1, y1,		1.f, 1.f
		};

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(vertices), vertices);
		vertex_buffer_.reset();
	}

	void ImagePlane2D::Draw(GLfloat x, GLfloat y)
	{
		if(texture_->id()) {

			texture_->bind();
			AbstractWindow::shaders->widget_image_program()->use();
			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_POSITION), x, y);
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_GAMMA), 0);

			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			texture_->reset();
			GLSLProgram::reset();

		}
	}

}
