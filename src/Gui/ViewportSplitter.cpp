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

#include <BlendInt/Gui/ViewportSplitter.hpp>
#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ViewportSplitterHandle::ViewportSplitterHandle(Orientation orientation)
	: AbstractViewport(),
	  orientation_(orientation),
	  vao_(0),
	  previous_viewport_(0),
	  next_viewport_(0)
	{
		if(orientation == Horizontal) {
			set_size(200, 1);
		} else {
			set_size(1, 200);
		}

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);

		VertexTool tool;
		tool.GenerateShadedVertices(size(),
				0,
				round_type(),
				round_radius());

		glGenVertexArrays(1, &vao_);
		buffer_.generate();

		glBindVertexArray(vao_);
		buffer_.bind();
		buffer_.set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_COORD),
				3, GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::reset();
		glBindVertexArray(0);
	}

	ViewportSplitterHandle::~ViewportSplitterHandle()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ViewportSplitterHandle::SetHandleWidget(Widget* widget)
	{
		if(widget_count()) {
			Clear();
		}

		if(PushBackSubWidget(widget)) {
			widget->SetPosition(0, 0);
			widget->Resize(size().width(),
					size().height());
		}
	}

	Size ViewportSplitterHandle::GetPreferredSize() const
	{
		Size preferred_size(1, 1);

		if(widget_count()) {
			preferred_size = first()->GetPreferredSize();
		}

		return preferred_size;
	}

	void ViewportSplitterHandle::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			float x = static_cast<float>(request.position()->x()  + offset_x());
			float y = static_cast<float>(request.position()->y()  + offset_y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)size().width(),
				y,
				y + (float)size().height(),
				100.f, -100.f);

			model_matrix_ = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));

			set_position(*request.position());
		}

		if(request.source() == this) {
			ReportPositionUpdate (request);
		}
	}

	void ViewportSplitterHandle::PerformSizeUpdate(
			const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			float x = static_cast<float>(position().x() + offset_x());
			float y = static_cast<float>(position().y() + offset_y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)request.size()->width(),
				y,
				y + (float)request.size()->height(),
				100.f, -100.f);

			VertexTool tool;
			tool.GenerateShadedVertices(*request.size(),
				0,
				round_type(),
				round_radius());

			buffer_.bind();
			buffer_.set_sub_data(0, tool.inner_size(), tool.inner_data());
			GLArrayBuffer::reset();

			set_size(*request.size());

			if(widget_count()) {
				first()->Resize(*request.size());
			}
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType ViewportSplitterHandle::Draw(Profile& profile)
	{
		glViewport(position().x(), position().y(), size().width(), size().height());
		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetUIProjectionMatrix(projection_matrix_);
		Shaders::instance->SetUIModelMatrix(model_matrix_);

		Shaders::instance->widget_program()->use();

		glUniform3f(Shaders::instance->location(Stock::WIDGET_POSITION),
				0.f, 0.f, 0.f);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_ANTI_ALIAS),
				0);

		glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.9f, 0.05f, 0.05f, 1.f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		glBindVertexArray(0);
		GLSLProgram::reset();

		for(AbstractWidget* p = first(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	ResponseType ViewportSplitterHandle::FocusEvent(bool focus)
	{
		return Ignore;
	}

	ResponseType ViewportSplitterHandle::CursorEnterEvent(bool entered)
	{
		return Ignore;
	}

	ResponseType ViewportSplitterHandle::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitterHandle::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitterHandle::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitterHandle::MousePressEvent(
			const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitterHandle::MouseReleaseEvent(
			const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitterHandle::MouseMoveEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	// --------------------------------

	ViewportSplitter::ViewportSplitter(Orientation orientation)
	: AbstractViewport(),
	  orientation_(orientation)
	{
		set_size(500, 500);
	}

	ViewportSplitter::~ViewportSplitter()
	{

	}

	void ViewportSplitter::PrependViewport(AbstractViewport* viewport)
	{
	}

	void ViewportSplitter::AppendViewport(AbstractViewport* viewport)
	{
		if(viewport && viewport->container() != this) {

			if(first() == 0) {
				PushBackSubWidget(viewport);
			} else {
				ViewportSplitterHandle* handle = 0;

				if(orientation_ == Horizontal) {
					handle = Manage(new ViewportSplitterHandle(Vertical));
				} else {
					handle = Manage(new ViewportSplitterHandle(Horizontal));
				}

				AbstractWidget* p = last();
				PushBackSubWidget(handle);
				handle->previous_viewport_ = dynamic_cast<AbstractViewport*>(p);
				handle->next_viewport_ = viewport;
				PushBackSubWidget(viewport);
			}

			AlignSubViewports(orientation_, size());
		}
	}

	void ViewportSplitter::Insert(int index, AbstractViewport* viewport)
	{
	}

	Size ViewportSplitter::GetPreferredSize() const
	{
		Size preferred_size;

		if(first() == 0) {
			preferred_size.set_width(400);
			preferred_size.set_height(400);
		} else {
			Size tmp;

			if (orientation_ == Horizontal) {
				for(AbstractWidget* p = first(); p; p = p->next())
				{
					if (p->visiable()) {
						tmp = p->GetPreferredSize();
						preferred_size.add_width(tmp.width());
						preferred_size.set_height(
										std::max(preferred_size.height(),
														tmp.height()));
					}
				}
			} else {
				for(AbstractWidget* p = first(); p; p = p->next())
				{
					if(p->visiable()) {
						tmp = p->GetPreferredSize();
						preferred_size.add_height(tmp.height());
						preferred_size.set_width(
										std::max(preferred_size.width(), tmp.width()));
					}
				}
			}

		}

		return preferred_size;
	}

	ResponseType ViewportSplitter::Draw(Profile& profile)
	{
		for(AbstractWidget* p = first(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	void ViewportSplitter::AlignSubViewports(Orientation orientation,
			const Size& size)
	{
		int room = GetAverageRoom(orientation, size);
		int x = position().x();

		if(orientation == Horizontal) {

			int y = position().y();
			int h = size.height();

			int i = 0;
			int handler_width = 0;
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(i % 2 == 0) {
					ResizeSubWidget(p, room, h);
					SetSubWidgetPosition(p, x, y);
					x = x + room;
				} else {
					handler_width = p->GetPreferredSize().width();
					ResizeSubWidget(p, handler_width, h);
					SetSubWidgetPosition(p, x, y);
					x = x + handler_width;
				}
				i++;
			}

		} else {

			int y = position().y() + size.height();
			int w = size.width();

			int i = 0;
			int handler_height = 0;
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(i % 2 == 0) {
					y = y - room;
					ResizeSubWidget(p, w, room);
					SetSubWidgetPosition(p, x, y);
				} else {
					handler_height = p->GetPreferredSize().height();
					y = y - handler_height;
					ResizeSubWidget(p, w, handler_height);
					SetSubWidgetPosition(p, x, y);
				}

				i++;
			}

		}

	}

	ResponseType ViewportSplitter::FocusEvent(bool focus)
	{
		return Ignore;
	}

	ResponseType ViewportSplitter::CursorEnterEvent(bool entered)
	{
		return Ignore;
	}

	ResponseType ViewportSplitter::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitter::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitter::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ViewportSplitter::MousePressEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		for(AbstractWidget* p = last(); p; p = p->previous()) {

			if(p->Contain(event.global_position())) {

				response = assign_mouse_press_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType ViewportSplitter::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		for(AbstractWidget* p = last(); p; p = p->previous()) {

			if(p->Contain(event.global_position())) {

				response = assign_mouse_release_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType ViewportSplitter::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		if(Contain(event.global_position())) {

			for(AbstractWidget* p = last(); p; p = p->previous()) {

				if(p->Contain(event.global_position())) {

					response = assign_mouse_move_event(p, event);
					if(response == Accept) break;
				}

			}

		} else {
			return Ignore;
		}

		return response;
	}

	int ViewportSplitter::GetAverageRoom(Orientation orientation,
			const Size& size)
	{
		int room = 0;

		if(orientation == Horizontal) {
			room = size.width();
		} else {
			room = size.height();
		}

		if(first() == 0) {
			return room;
		}

		Size prefer;
		int space = 0;
		int sum = 0;

		AbstractWidget* p = first()->next();
		sum += 1;

		while (p) {
			prefer = p->GetPreferredSize();
			if(orientation == Horizontal) {
				space = prefer.width();
			} else {
				space = prefer.height();
			}

			p = p->next()->next();
			sum += 2;
		}

		room = room - space;
		room = room / ((sum + 1) / 2);

		return room;
	}

}
