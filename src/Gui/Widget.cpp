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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Widget::Widget()
	: AbstractWidget()
	{
		set_size(120, 80);

		InitializeWidgetOnce();
	}

	Widget::~Widget()
	{
		glDeleteVertexArrays(2, vao_);
	}

	ResponseType Widget::Draw(Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program_ext();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position_ext(),
				(float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma_ext(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias_ext(),
				0);

		glUniform4f(Shaders::instance->triangle_uniform_color_ext(), 0.f, 0.2f, 0.5f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_shade_ext(),
				0.f, 0.f, 0.f, 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_antialias_ext(), 1);
		glUniform4f(Shaders::instance->triangle_uniform_color_ext(), 0.f, 0.f, 0.f, 1.f);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		program->reset();
		return Accept;
	}

	ResponseType Widget::CursorEnterEvent(bool entered)
	{
		return Accept;
	}

	ResponseType Widget::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}
	
	ResponseType Widget::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Widget::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	void Widget::InitializeWidgetOnce()
	{
		set_round_type(RoundAll);

		VertexTool tool;
		tool.GenerateVertices(size(),
				DefaultBorderWidth(),
				round_type(),
				round_radius(),
				Vertical,
				25,
				-15);

		glGenVertexArrays(2, vao_);

		glBindVertexArray(vao_[0]);
		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord_ext());
		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_shade_ext());

		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord_ext(),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->triangle_attrib_shade_ext(),
				4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord_ext());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord_ext(),
				2, GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::reset();
		glBindVertexArray(0);
	}

	// ----------------------------------------------------------

	Container::Container ()
	{
	}

	Container::~Container ()
	{
	}

	ResponseType Container::Draw (Profile& profile)
	{
		return Ignore;
	}

	ResponseType Container::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Container::KeyPressEvent (
	        const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::MousePressEvent (
	        const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::MouseReleaseEvent (
	        const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::MouseMoveEvent (
	        const MouseEvent& event)
	{
		return Ignore;
	}

}

