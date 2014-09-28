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

#include <BlendInt/Gui/Screen.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Screen::Screen ()
	: AbstractScreen(),
	  focused_widget_(0),
	  top_hovered_widget_(0),
	  custom_focused_widget_(false)
	{
		set_size(500, 400);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);
	}

	Screen::~Screen()
	{
		if(focused_widget_) {
			set_widget_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Screen::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		ClearHoverWidgets();
	}

	void Screen::Setup(Widget* widget)
	{
		if(widget == 0) return;

		if(widget->container() == this) return;

		if(subs_count() > 0) ClearSubWidgets();

		Resize(widget->size());

		if(PushBackSubWidget(widget)) {
			widget->SetPosition(0, 0);
		}
	}

	void Screen::Setup(Container* container)
	{
		if(container == 0) return;

		if(container->container() == this) return;

		if(subs_count() > 0) ClearSubWidgets();

		Resize(container->size());

		if(PushBackSubWidget(container)) {
			container->SetPosition(0, 0);
		}
	}

	void Screen::SetFocused(AbstractWidget* widget)
	{
		custom_focused_widget_ = true;

		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Screen::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, true);
			events()->connect(focused_widget_->destroyed(), this, &Screen::OnFocusedWidgetDestroyed);
		}
	}

	Size Screen::GetPreferredSize() const
	{
		Size prefer;

		if(subs_count()) {
			int minx = 0;
			int miny = 0;
			int maxx = 0;
			int maxy = 0;

			for(AbstractWidget* p = first_sub_widget(); p; p = p->next()) {
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

	bool Screen::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		if(request.source()->container() == this) {
			return false;
		} else {
			return true;
		}
	}

	bool Screen::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		if(request.source()->container() == this) {
			return false;
		} else {
			return true;
		}
	}

	void Screen::PerformPositionUpdate(
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

	void Screen::PerformSizeUpdate (const SizeUpdateRequest& request)
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

			set_size(*request.size());

			if (subs_count()) {
				assert(subs_count() == 1);
				FillSingleWidget(0, 0, 0, size().width(), size().height());
			}

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Screen::PreDraw(Profile& profile)
	{
		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		profile.set_origin(
				position().x() + offset().x(),
				position().y() + offset().y()
		);

		Shaders::instance->SetUIProjectionMatrix(projection_matrix_);
		Shaders::instance->SetUIModelMatrix(model_matrix_);
	}

	ResponseType Screen::Draw (Profile& profile)
	{
		for(AbstractWidget* p = first_sub_widget(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	void Screen::PostDraw(Profile& profile)
	{
		set_refresh(false);
	}

	ResponseType Screen::FocusEvent(bool focus)
	{
		return Ignore;
	}

	ResponseType Screen::CursorEnterEvent(bool entered)
	{
		if(entered) {
		} else {
			ClearHoverWidgets();
		}

		return Ignore;
	}

	ResponseType Screen::KeyPressEvent(const KeyEvent& event)
	{
		set_event_viewport(event);

		ResponseType response = Ignore;

		if(focused_widget_) {
			assign_key_press_event(focused_widget_, event);
		}

		return response;
	}

	ResponseType Screen::ContextMenuPressEvent(const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Screen::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Screen::MousePressEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_viewport(event);

		CheckAndUpdateHoverWidget(event);

		if(top_hovered_widget_) {

			AbstractWidget* widget = 0;	// widget may be focused

			custom_focused_widget_ = false;

			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - position().x() - offset().x(),
					event.position().y() - position().y() - offset().y());

			retval = DispatchMousePressEvent(top_hovered_widget_, event);

			if(retval == Accept) {
				widget = top_hovered_widget_;
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

	ResponseType Screen::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_viewport(event);

		CheckAndUpdateHoverWidget(event);

		if(focused_widget_) {
			retval = assign_mouse_release_event(focused_widget_, event);
		}

		if(retval == Accept) return retval;

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType Screen::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_viewport(event);

		// TODO: make sure focused widget is still in this viewport
		if(focused_widget_) {
			retval = assign_mouse_move_event(focused_widget_, event);
		}

		if(retval == Accept) {
			CheckAndUpdateHoverWidget(event);
			return retval;
		}

		CheckAndUpdateHoverWidget(event);

		if(top_hovered_widget_) {
			retval = assign_mouse_move_event(top_hovered_widget_, event);
		}

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	bool Screen::CheckAndUpdateHoverWidget(const MouseEvent& event)
	{
		Point local_cursor;

		// find the new top hovered widget
		if (top_hovered_widget_) {

			AbstractWidget* parent = top_hovered_widget_->container();

			Point parent_position = parent->GetGlobalPosition();

			bool not_hover_through = event.position().x() < parent_position.x() ||
					event.position().y() < parent_position.y() ||
					event.position().x() > (parent_position.x() + parent->size().width()) ||
					event.position().y() > (parent_position.y() + parent->size().height());

			local_cursor.set_x(event.position().x() - parent_position.x() - parent->offset().x());
			local_cursor.set_y(event.position().y() - parent_position.y() - parent->offset().y());

			if(!not_hover_through) {

				if(top_hovered_widget_->Contain(local_cursor)) {

					AbstractWidget* orig = top_hovered_widget_;

					UpdateHoverWidgetSubs(local_cursor);

					if(orig != top_hovered_widget_) {
						orig->destroyed().disconnectOne(this,
								&Screen::OnHoverWidgetDestroyed);
						events()->connect(top_hovered_widget_->destroyed(), this,
						        &Screen::OnHoverWidgetDestroyed);
					}

				} else {

					top_hovered_widget_->destroyed ().disconnectOne (this,
								&Screen::OnHoverWidgetDestroyed);
					set_widget_hover_event(top_hovered_widget_, false);

					// find which contianer contains cursor position
					while (parent) {

						if (parent == this) {	// FIXME: the widget may be mvoed to another context
							parent = 0;
							break;
						}

						local_cursor.set_x(local_cursor.x() + parent->position().x() + parent->offset().x());
						local_cursor.set_y(local_cursor.y() + parent->position().y() + parent->offset().y());

						if (parent->Contain(local_cursor)) break;

						parent = parent->container();
					}

					top_hovered_widget_ = parent;

					if(top_hovered_widget_) {
						UpdateHoverWidgetSubs(local_cursor);
						events()->connect(top_hovered_widget_->destroyed(), this,
						        &Screen::OnHoverWidgetDestroyed);
					}

				}

			} else {

				top_hovered_widget_->destroyed().disconnectOne(this,
					        &Screen::OnHoverWidgetDestroyed);
				set_widget_hover_event(top_hovered_widget_, false);

				// find which contianer contains cursor position
				parent = parent->container();
				while (parent) {

					if (parent == this) {	// FIXME: the widget may be mvoed to another context
						parent = 0;
						break;
					}

					local_cursor.set_x(local_cursor.x() + parent->position().x() + parent->offset().x());
					local_cursor.set_y(local_cursor.y() + parent->position().y() + parent->offset().y());

					if(IsHoverThroughExt(parent, event.position())) break;
					parent = parent->container();
				}

				top_hovered_widget_ = parent;
				if(top_hovered_widget_) {
					UpdateHoverWidgetSubs(local_cursor);
					events()->connect(top_hovered_widget_->destroyed(), this,
					        &Screen::OnHoverWidgetDestroyed);
				}

			}

		} else {

			local_cursor.set_x(event.position().x() - position().x() - offset().x());
			local_cursor.set_y(event.position().y() - position().y() - offset().y());

			for(AbstractWidget* p = last_sub_widget(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(local_cursor)) {

					top_hovered_widget_ = p;
					set_widget_hover_event(top_hovered_widget_, true);

					break;
				}
			}

			if(top_hovered_widget_) {
				UpdateHoverWidgetSubs(local_cursor);
				events()->connect(top_hovered_widget_->destroyed(), this,
				        &Screen::OnHoverWidgetDestroyed);
			}

		}

		return top_hovered_widget_ != 0;
	}

	void Screen::UpdateHoverWidgetSubs(Point& cursor)
	{
		cursor.set_x (
				cursor.x () - top_hovered_widget_->position ().x ()
						- top_hovered_widget_->offset ().x ());
		cursor.set_y (
				cursor.y () - top_hovered_widget_->position ().y ()
						- top_hovered_widget_->offset ().y ());

		for (AbstractWidget* p = top_hovered_widget_->last_sub_widget (); p;
				p = p->previous ()) {
			if (p->visiable () && p->Contain (cursor)) {

				top_hovered_widget_ = p;
				set_widget_hover_event (top_hovered_widget_, true);

				UpdateHoverWidgetSubs (cursor);
				break;
			}
		}

	}

	void Screen::OnFocusedWidgetDestroyed(AbstractWidget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Screen::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void Screen::OnHoverWidgetDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(top_hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Screen::OnHoverWidgetDestroyed);

		top_hovered_widget_ = 0;
	}

	void Screen::ClearHoverWidgets()
	{
		if(top_hovered_widget_) {

			top_hovered_widget_->destroyed().disconnectOne(this, &Screen::OnHoverWidgetDestroyed);

			while (top_hovered_widget_ && top_hovered_widget_ != this) {
				set_widget_hover_event(top_hovered_widget_, false);
				top_hovered_widget_ = top_hovered_widget_->container();
			}

			if(top_hovered_widget_ == this)
				top_hovered_widget_ = 0;

		}
	}

}
