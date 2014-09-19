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

#include <BlendInt/Gui/Viewport.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Viewport::Viewport ()
	: AbstractViewport(),
	  focused_(0),
	  top_hovered_(0),
	  custom_focused_widget_(false),
	  vao_(0)
	{
		set_size(500, 400);

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

	Viewport::~Viewport()
	{
		glDeleteVertexArrays(1, &vao_);

		if(focused_) {
			set_widget_focus_status(focused_, false);
			focused_->destroyed().disconnectOne(this, &Viewport::OnFocusedWidgetDestroyed);
			focused_ = 0;
		}

		if(top_hovered_) {
			set_widget_hover_status(top_hovered_, false);
			top_hovered_->destroyed().disconnectOne(this, &Viewport::OnHoverWidgetDestroyed);
			top_hovered_ = 0;
		}
	}

	void Viewport::AddWidget(Widget* widget)
	{
		if(PushBackSubWidget(widget)) {
			// set position
		}
	}

	void Viewport::SetFocused(AbstractWidget* widget)
	{
		custom_focused_widget_ = true;

		if(focused_ == widget)
			return;

		if (focused_) {
			set_widget_focus_event(focused_, false);
			focused_->destroyed().disconnectOne(this, &Viewport::OnFocusedWidgetDestroyed);
		}

		focused_ = widget;
		if (focused_) {
			set_widget_focus_event(focused_, true);
			events()->connect(focused_->destroyed(), this, &Viewport::OnFocusedWidgetDestroyed);
		}
	}

	Size Viewport::GetPreferredSize() const
	{
		Size prefer;

		if(widget_count()) {
			int minx = 0;
			int miny = 0;
			int maxx = 0;
			int maxy = 0;

			for(AbstractWidget* p = first(); p; p = p->next()) {
				minx = std::min(p->position().x(), minx);
				miny = std::min(p->position().y(), miny);
				maxx = std::max(p->position().x() + p->size().width(), maxx);
				maxy = std::max(p->position().y() + p->size().height(), maxy);

				prefer.reset(maxx - minx, maxy - miny);
			}

		} else {
			prefer.reset(500, 400);
		}

		return prefer;
	}

	void Viewport::PerformPositionUpdate(
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

	void Viewport::PerformSizeUpdate (const SizeUpdateRequest& request)
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
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType Viewport::Draw (Profile& profile)
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

		glUniform4f(Shaders::instance->location(Stock::WIDGET_COLOR), 0.4f, 0.65f, 0.2f, 1.f);

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

	ResponseType Viewport::FocusEvent(bool focus)
	{
		return Ignore;
	}

	ResponseType Viewport::CursorEnterEvent(bool entered)
	{
		return Ignore;
	}

	ResponseType Viewport::KeyPressEvent(const KeyEvent& event)
	{
		set_event_viewport(event);

		ResponseType response = Ignore;

		if(focused_) {
			assign_key_press_event(focused_, event);
		}

		return response;
	}

	ResponseType Viewport::ContextMenuPressEvent(const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport::MousePressEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_viewport(event);

		CheckAndUpdateHoverWidget(event);

		if(top_hovered_) {

			AbstractWidget* widget = 0;	// widget may be focused

			custom_focused_widget_ = false;

			retval = DispatchMousePressEvent(top_hovered_, event);

			if(retval == Accept) {
				widget = top_hovered_;
			}

			if(!custom_focused_widget_) {
				SetFocused(widget);
			}
			custom_focused_widget_ = false;

		}

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType Viewport::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_viewport(event);

		CheckAndUpdateHoverWidget(event);

		if(focused_) {
			retval = assign_mouse_release_event(focused_, event);
		}

		if(retval == Accept) return retval;

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType Viewport::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_viewport(event);

		if(Contain(event.global_position())) {
			const_cast<MouseEvent&>(event).set_local_position(
					event.global_position().x() - position().x(),
					event.global_position().y() - position().y());
		} else {
			return Ignore;
		}

		CheckAndUpdateHoverWidget(event);

		if(top_hovered_) {
			retval = assign_mouse_move_event(top_hovered_, event);
		}

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	bool Viewport::CheckAndUpdateHoverWidget(const MouseEvent& event)
	{
		if (top_hovered_) {

			if (IsHoverThroughExt (top_hovered_->container(),
					event.global_position ())) {

				if (top_hovered_->Contain (event.local_position ())) {

					AbstractWidget* orig = top_hovered_;
					UpdateHoverWidgetSubs (event);

					if (orig != top_hovered_) {
						orig->destroyed ().disconnectOne (this,
								&Viewport::OnHoverWidgetDestroyed);
						events ()->connect (top_hovered_->destroyed (),
								this, &Viewport::OnHoverWidgetDestroyed);
					}

				} else {

					top_hovered_->destroyed ().disconnectOne (this,
							&Viewport::OnHoverWidgetDestroyed);
					set_widget_hover_event(top_hovered_, false);

					// find which contianer contains cursor position
					while (top_hovered_->container ()) {

						if (top_hovered_->container () == this) {	// FIXME: the widget may be mvoed to another context
							top_hovered_ = 0;
							break;
						} else {
							top_hovered_ =
									top_hovered_->container ();

							if (top_hovered_->Contain (
									event.local_position ())) {
								break;
							}
						}
					}

					if (top_hovered_) {
						UpdateHoverWidgetSubs (event);
						events()->connect(top_hovered_->destroyed (),
								this, &Viewport::OnHoverWidgetDestroyed);
					}
				}

			} else {

				top_hovered_->destroyed().disconnectOne(this,
				        &Viewport::OnHoverWidgetDestroyed);
				set_widget_hover_event(top_hovered_, false);

				// find which contianer contains cursor position
				while (top_hovered_->container()) {

					if (top_hovered_->container() == this) {	// FIXME: the widget may be mvoed to another context
						top_hovered_ = 0;
						break;
					} else {
						top_hovered_ = top_hovered_->container();

						if (IsHoverThroughExt(top_hovered_, event.local_position())) {
							break;
						}
					}
				}

				if (top_hovered_) {
					UpdateHoverWidgetSubs(event);
					events()->connect(top_hovered_->destroyed(), this, &Viewport::OnHoverWidgetDestroyed);
				}

			}

		} else {

			for(AbstractWidget* p = last(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(event.local_position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					top_hovered_ = p;
					set_widget_hover_event(top_hovered_, true);

					break;
				}
			}

			if(top_hovered_) {

				UpdateHoverWidgetSubs(event);

				events()->connect(top_hovered_->destroyed(), this,
				        &Viewport::OnHoverWidgetDestroyed);
			}

		}

		return top_hovered_ != 0;
	}

	void Viewport::UpdateHoverWidgetSubs(const MouseEvent& event)
	{
		AbstractContainer* parent = dynamic_cast<AbstractContainer*>(top_hovered_);

		if (parent) {

			for(AbstractWidget* p = parent->last(); p; p = p->previous())
			{
				if(p->visiable() && p->Contain(event.local_position())) {

					top_hovered_ = p;
					set_widget_hover_event(top_hovered_, true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}

		}
	}

	void Viewport::OnFocusedWidgetDestroyed(AbstractWidget* widget)
	{
		assert(focused_ == widget);
		assert(widget->focused());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Viewport::OnFocusedWidgetDestroyed);

		focused_ = 0;
	}

	void Viewport::OnHoverWidgetDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(top_hovered_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Viewport::OnHoverWidgetDestroyed);

		top_hovered_ = 0;
	}

	void Viewport::ClearHoverWidgets()
	{
		if(top_hovered_) {

			top_hovered_->destroyed().disconnectOne(this, &Viewport::OnHoverWidgetDestroyed);

			while (top_hovered_ && top_hovered_ != this) {
				set_widget_hover_event(top_hovered_, false);
				top_hovered_ = top_hovered_->container();
			}

			if(top_hovered_ == this)
				top_hovered_ = 0;

		}
	}

}
