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
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/FloatingFrame.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FloatingFrame::FloatingFrame()
	: AbstractFrame(),
	  vao_(0),
	  pressed_(false)
	{
		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		buffer_.generate();
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

	FloatingFrame::~FloatingFrame()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void FloatingFrame::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			std::vector<GLfloat> inner_verts;
			GenerateVertices(*request.size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

			buffer_.bind();
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.reset();

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void FloatingFrame::PreDraw(Profile& profile)
	{
	}

	ResponseType FloatingFrame::Draw(Profile& profile)
	{
		Shaders::instance->frame_inner_program()->use();

		glUniform2f(Shaders::instance->location(Stock::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform1i(Shaders::instance->location(Stock::FRAME_INNER_GAMMA), 0);

		if(hover()) {
			glUniform4f(Shaders::instance->location(Stock::FRAME_INNER_COLOR), 0.75f, 0.15f, 0.15f, 0.6f);
		} else {
			glUniform4f(Shaders::instance->location(Stock::FRAME_INNER_COLOR), 0.75f, 0.75f, 0.75f, 0.6f);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		GLSLProgram::reset();

		return subs_count() ? Ignore : Accept;
	}

	void FloatingFrame::PostDraw(Profile& profile)
	{
	}

	void FloatingFrame::FocusEvent(bool focus)
	{
	}

	void FloatingFrame::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void FloatingFrame::MouseHoverOutEvent(const MouseEvent& event)
	{
	}

	ResponseType FloatingFrame::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType FloatingFrame::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FloatingFrame::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FloatingFrame::MousePressEvent(const MouseEvent& event)
	{
		set_event_frame(event);

		last_ = position();
		cursor_ = event.position();
		pressed_ = true;

		return Accept;
	}

	ResponseType FloatingFrame::MouseReleaseEvent(const MouseEvent& event)
	{
		if (pressed_) {
			pressed_ = false;
		}

		return Accept;
	}

	void FloatingFrame::DispatchHoverEvent(const MouseEvent& event)
	{
	}

	ResponseType FloatingFrame::MouseMoveEvent(const MouseEvent& event)
	{
		if(pressed_) {

			int ox = event.position().x() - cursor_.x();
			int oy = event.position().y() - cursor_.y();

			set_position(last_.x() + ox, last_.y() + oy);

			Refresh();

			return Accept;

		}

		return Ignore;
	}

}
