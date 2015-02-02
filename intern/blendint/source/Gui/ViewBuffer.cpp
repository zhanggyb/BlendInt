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

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <BlendInt/Gui/ViewBuffer.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	// FIXME: the default constructor has no effect
	ViewBuffer::ViewBuffer ()
	: AbstractForm(),
	  vao_(0)
	{
		glGenVertexArrays(1, &vao_);
		vbo_.generate();

		GLfloat vertices[] = {
			// coord											uv
			0.f, 0.f,											0.f, 0.f,
			(float)size().width(), 0.f,							1.f, 0.f,
			0.f, (float)size().height(),						0.f, 1.f,
			(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		glBindVertexArray(vao_);

		vbo_.bind();
		vbo_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(AttributeCoord);
		glEnableVertexAttribArray(AttributeUV);
		glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE,
		        sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(AttributeUV, 2,
		GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vbo_.reset();
	}

	ViewBuffer::ViewBuffer (int width, int height)
	: AbstractForm(width, height),
	  vao_(0)
	{
		glGenVertexArrays(1, &vao_);
		vbo_.generate();

		GLfloat vertices[] = {
			// coord											uv
			0.f, 0.f,											0.f, 0.f,
			(float)size().width(), 0.f,							1.f, 0.f,
			0.f, (float)size().height(),						0.f, 1.f,
			(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		glBindVertexArray(vao_);

		vbo_.bind();
		vbo_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(AttributeCoord);
		glEnableVertexAttribArray(AttributeUV);
		glVertexAttribPointer(AttributeCoord,
							  2,
							  GL_FLOAT,
							  GL_FALSE,
							  sizeof(GLfloat) * 4,
							  BUFFER_OFFSET(0));
		glVertexAttribPointer(AttributeUV,
							  2,
							  GL_FLOAT,
							  GL_FALSE,
							  sizeof(GLfloat) * 4,
							  BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vbo_.reset();
	}

	ViewBuffer::~ViewBuffer ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ViewBuffer::Draw () const
	{
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		texture_.bind();
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

#ifdef DEBUG

	void ViewBuffer::SaveToFile(const char* name)
	{
		texture_.bind();
		texture_.WriteToFile(name);
	}

#endif

	void ViewBuffer::PerformSizeUpdate (const Size& size)
	{
		set_size(size);

		vbo_.bind();

		float* ptr = (float*)vbo_.map();
		*(ptr + 4) = (float)this->size().width();
		*(ptr + 9) = (float)this->size().height();
		*(ptr + 12) = (float)this->size().width();
		*(ptr + 13) = (float)this->size().height();
		vbo_.unmap();

		vbo_.reset();
	}

}
