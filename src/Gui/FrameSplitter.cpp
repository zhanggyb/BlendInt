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

#include <BlendInt/Gui/FrameSplitter.hpp>
#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FrameSplitterHandle::FrameSplitterHandle(Orientation orientation)
	: AbstractFrame(),
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

	FrameSplitterHandle::~FrameSplitterHandle()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void FrameSplitterHandle::SetHandleWidget(Widget* widget)
	{
		if(subs_count()) {
			ClearSubWidgets();
		}

		if(PushBackSubWidget(widget)) {
			widget->SetPosition(0, 0);
			widget->Resize(size().width(),
					size().height());
		}
	}

	Size FrameSplitterHandle::GetPreferredSize() const
	{
		Size preferred_size(1, 1);

		if(subs_count()) {
			preferred_size = first_child()->GetPreferredSize();
		}

		return preferred_size;
	}

	void FrameSplitterHandle::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			float x = static_cast<float>(request.position()->x()  + offset().x());
			float y = static_cast<float>(request.position()->y()  + offset().y());

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

	void FrameSplitterHandle::PerformSizeUpdate(
			const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			float x = static_cast<float>(position().x() + offset().x());
			float y = static_cast<float>(position().y() + offset().y());

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

			if(subs_count()) {
				first_child()->Resize(*request.size());
			}
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType FrameSplitterHandle::Draw(Profile& profile)
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

		if(hover()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.9f, 0.05f, 0.05f, 1.f);
		} else {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.6f, 0.05f, 0.05f, 1.f);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		glBindVertexArray(0);
		GLSLProgram::reset();

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	ResponseType FrameSplitterHandle::FocusEvent(bool focus)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::CursorEnterEvent(bool entered)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::MousePressEvent(
			const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::MouseReleaseEvent(
			const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::MouseMoveEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	// --------------------------------

	FrameSplitter::FrameSplitter(Orientation orientation)
	: AbstractFrame(),
	  orientation_(orientation),
	  hover_(0)
	{
		set_size(500, 500);
	}

	FrameSplitter::~FrameSplitter()
	{

	}

	void FrameSplitter::PrependViewport(AbstractFrame* viewport)
	{
	}

	void FrameSplitter::AppendViewport(AbstractFrame* viewport)
	{
		if(viewport && viewport->parent() != this) {

			if(first_child() == 0) {
				PushBackSubWidget(viewport);
			} else {
				FrameSplitterHandle* handle = 0;

				if(orientation_ == Horizontal) {
					handle = Manage(new FrameSplitterHandle(Vertical));
				} else {
					handle = Manage(new FrameSplitterHandle(Horizontal));
				}

				AbstractWidget* p = last_child();
				PushBackSubWidget(handle);
				handle->previous_viewport_ = dynamic_cast<AbstractFrame*>(p);
				handle->next_viewport_ = viewport;
				PushBackSubWidget(viewport);
			}

			AlignSubViewports(orientation_, size());
		}
	}

	void FrameSplitter::Insert(int index, AbstractFrame* viewport)
	{
	}

	Size FrameSplitter::GetPreferredSize() const
	{
		Size preferred_size;

		if(first_child() == 0) {
			preferred_size.set_width(400);
			preferred_size.set_height(400);
		} else {
			Size tmp;

			if (orientation_ == Horizontal) {
				for(AbstractWidget* p = first_child(); p; p = p->next())
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
				for(AbstractWidget* p = first_child(); p; p = p->next())
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

	ResponseType FrameSplitter::Draw(Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	void FrameSplitter::AlignSubViewports(Orientation orientation,
			const Size& size)
	{
		int room = GetAverageRoom(orientation, size);
		int x = position().x();

		if(orientation == Horizontal) {

			int y = position().y();
			int h = size.height();

			int i = 0;
			int handler_width = 0;
			for(AbstractWidget* p = first_child(); p; p = p->next())
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
			for(AbstractWidget* p = first_child(); p; p = p->next())
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

	ResponseType FrameSplitter::FocusEvent(bool focus)
	{
		return Ignore;
	}

	ResponseType FrameSplitter::CursorEnterEvent(bool entered)
	{
		if(entered) {
			// TODO: do sth.
		} else {
			if(hover_) {
				set_widget_hover_event(hover_, false);
				hover_->destroyed().disconnectOne(this, &FrameSplitter::OnHoverViewportDestroyed);
				hover_ = 0;
			}
		}
		return Ignore;
	}

	ResponseType FrameSplitter::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitter::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitter::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitter::MousePressEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		for(AbstractWidget* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = assign_mouse_press_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType FrameSplitter::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		for(AbstractWidget* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = assign_mouse_release_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType FrameSplitter::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		AbstractFrame* original_hover = hover_;

		if(hover_) {
			if(!hover_->Contain(event.position())) {

				hover_ = 0;
				for(AbstractWidget* p = last_child(); p; p = p->previous()) {
					if(p->Contain(event.position())) {
						hover_ = dynamic_cast<AbstractFrame*>(p);
						break;
					}
				}

			}
		} else {

			for(AbstractWidget* p = last_child(); p; p = p->previous()) {
				if(p->Contain(event.position())) {
					hover_ = dynamic_cast<AbstractFrame*>(p);
					break;
				}
			}

		}

		if(original_hover != hover_) {

			if(original_hover) {
				set_widget_hover_event(original_hover, false);
				original_hover->destroyed().disconnectOne(this, &FrameSplitter::OnHoverViewportDestroyed);
			}

			if(hover_) {
				set_widget_hover_event(hover_, true);
				events()->connect(hover_->destroyed(), this, &FrameSplitter::OnHoverViewportDestroyed);
			}

		}

		if(hover_) {
			response = assign_mouse_move_event(hover_, event);
		}

		return response;
	}

	int FrameSplitter::GetAverageRoom(Orientation orientation,
			const Size& size)
	{
		int room = 0;

		if(orientation == Horizontal) {
			room = size.width();
		} else {
			room = size.height();
		}

		if(first_child() == 0) {
			return room;
		}

		Size prefer;
		int space = 0;
		int sum = 0;

		AbstractWidget* p = first_child()->next();
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

	void FrameSplitter::OnHoverViewportDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hover_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &FrameSplitter::OnHoverViewportDestroyed);

		hover_ = 0;
	}

}
