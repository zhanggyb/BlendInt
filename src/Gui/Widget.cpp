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
		RefPtr<GLSLProgram> program = Shaders::instance->widget_program();
		program->use();

		glUniform3f(Shaders::instance->location(Stock::WIDGET_POSITION),
				(float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS),
				0);

		glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.f, 0.2f, 0.5f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS), 1);
		glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.f, 0.f, 0.f, 1.f);

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
		tool.GenerateShadedVertices(size(),
				DefaultBorderWidth(),
				round_type(),
				round_radius());
		/*
		tool.GenerateVertices(size(),
				DefaultBorderWidth(),
				round_type(),
				round_radius(),
				Vertical,
				25,
				-25);
		*/

		glGenVertexArrays(2, vao_);
		buffers_.generate();

		glBindVertexArray(vao_[0]);
		buffers_.bind(0);
		buffers_.set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffers_.bind(1);
		buffers_.set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

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

