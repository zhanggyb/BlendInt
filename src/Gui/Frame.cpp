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

#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Frame::Frame ()
	: AbstractFrame(),
	  focused_widget_(0),
	  top_hovered_widget_(0),
	  cursor_followed_widget_(0),
	  custom_focused_widget_(false)
	{
		set_size(500, 400);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);
	}

	Frame::~Frame()
	{
		if(focused_widget_) {
			set_widget_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Frame::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		ClearHoverWidgets();

		SetCursorFollowedWidget(0);
	}

	void Frame::Setup(Widget* widget)
	{
		if(widget == 0) return;

		if(widget->parent() == this) return;

		if(subs_count() > 0) ClearSubWidgets();

		Resize(widget->size());

		if(PushBackSubWidget(widget)) {
			widget->SetPosition(0, 0);
		}
	}

	void Frame::SetFocused(Widget* widget)
	{
		custom_focused_widget_ = true;

		if(focused_widget_ == widget)
			return;

		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &Frame::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			set_widget_focus_event(focused_widget_, true);
			events()->connect(focused_widget_->destroyed(), this, &Frame::OnFocusedWidgetDestroyed);
		}
	}

	void Frame::SetCursorFollowedWidget(Widget* widget)
	{
		if(cursor_followed_widget_ == widget) return;

		if(cursor_followed_widget_) {
			cursor_followed_widget_->destroyed().disconnectOne(this, &Frame::OnCursorFollowedWidgetDestroyed);
		}

		cursor_followed_widget_ = widget;
		if(cursor_followed_widget_) {
			events()->connect(cursor_followed_widget_->destroyed(), this, &Frame::OnCursorFollowedWidgetDestroyed);
		}
	}

	bool Frame::IsExpandX() const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool Frame::IsExpandY() const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size Frame::GetPreferredSize() const
	{
		Size prefer;

		if(subs_count()) {
			int minx = 0;
			int miny = 0;
			int maxx = 0;
			int maxy = 0;

			for(AbstractWidget* p = first_child(); p; p = p->next()) {
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

	void Frame::DispatchHoverEvent(const MouseEvent& event)
	{
		set_event_frame(event);

		// find the new top hovered widget
		if (top_hovered_widget_) {

			AbstractWidget* parent = top_hovered_widget_->parent();

			Point parent_position = parent->GetGlobalPosition();

			bool not_hover_through = event.position().x() < parent_position.x() ||
					event.position().y() < parent_position.y() ||
					event.position().x() > (parent_position.x() + parent->size().width()) ||
					event.position().y() > (parent_position.y() + parent->size().height());

			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - parent_position.x() - parent->offset().x(),
					event.position().y() - parent_position.y() - parent->offset().y());

			if(!not_hover_through) {

				if(top_hovered_widget_->Contain(event.local_position())) {

					Widget* orig = top_hovered_widget_;

					DispatchMouseHoverEventInSubs(event);

					if(orig != top_hovered_widget_) {
						orig->destroyed().disconnectOne(this,
								&Frame::OnHoverWidgetDestroyed);
						events()->connect(top_hovered_widget_->destroyed(), this,
						        &Frame::OnHoverWidgetDestroyed);
					}

				} else {

					top_hovered_widget_->destroyed ().disconnectOne (this,
								&Frame::OnHoverWidgetDestroyed);
					set_widget_mouse_hover_out_event(top_hovered_widget_, event);

					// find which contianer contains cursor position
					while (parent) {

						if (parent == this) {	// FIXME: the widget may be mvoed to another context
							parent = 0;
							break;
						}

						const_cast<MouseEvent&>(event).set_local_position(
								event.local_position().x() + parent->position().x() + parent->offset().x(),
								event.local_position().y() + parent->position().y() + parent->offset().y());

						if (parent->Contain(event.local_position())) break;

						parent = parent->parent();
					}

					top_hovered_widget_ = dynamic_cast<Widget*>(parent);

					if(top_hovered_widget_) {
						DispatchMouseHoverEventInSubs(event);
						events()->connect(top_hovered_widget_->destroyed(), this,
						        &Frame::OnHoverWidgetDestroyed);
					}

				}

			} else {

				top_hovered_widget_->destroyed().disconnectOne(this,
					        &Frame::OnHoverWidgetDestroyed);
				set_widget_mouse_hover_out_event(top_hovered_widget_, event);

				// find which contianer contains cursor position
				parent = parent->parent();
				while (parent) {

					if (parent == this) {	// FIXME: the widget may be mvoed to another context
						parent = 0;
						break;
					}

					const_cast<MouseEvent&>(event).set_local_position(
							event.local_position().x() + parent->position().x() + parent->offset().x(),
							event.local_position().y() + parent->position().y() + parent->offset().y());

					if(IsHoverThroughExt(parent, event.position())) break;
					parent = parent->parent();
				}

				top_hovered_widget_ = dynamic_cast<Widget*>(parent);
				if(top_hovered_widget_) {
					DispatchMouseHoverEventInSubs(event);
					events()->connect(top_hovered_widget_->destroyed(), this,
					        &Frame::OnHoverWidgetDestroyed);
				}

			}

		} else {

			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - position().x() - offset().x(),
					event.position().y() - position().y() - offset().y());

			for(AbstractWidget* p = last_child(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(event.local_position())) {

					top_hovered_widget_ = dynamic_cast<Widget*>(p);
					set_widget_mouse_hover_in_event(top_hovered_widget_, event);

					break;
				}
			}

			if(top_hovered_widget_) {
				DispatchMouseHoverEventInSubs(event);
				events()->connect(top_hovered_widget_->destroyed(), this,
				        &Frame::OnHoverWidgetDestroyed);
			}

		}

	}

	bool Frame::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		if(request.source()->parent() == this) {
			return false;
		} else {
			return true;
		}
	}

	bool Frame::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		if(request.source()->parent() == this) {
			return false;
		} else {
			return true;
		}
	}

	void Frame::PerformPositionUpdate(
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

	void Frame::PerformSizeUpdate (const SizeUpdateRequest& request)
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

	void Frame::PreDraw(Profile& profile)
	{
		assign_frame(profile);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
		Shaders::instance->SetWidgetModelMatrix(model_matrix_);
	}

	ResponseType Frame::Draw (Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	void Frame::PostDraw(Profile& profile)
	{
		glDisable(GL_SCISSOR_TEST);
	}

	void Frame::FocusEvent(bool focus)
	{
	}

	void Frame::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void Frame::MouseHoverOutEvent(const MouseEvent& event)
	{
		ClearHoverWidgets();
	}

	ResponseType Frame::KeyPressEvent(const KeyEvent& event)
	{
		set_event_frame(event);

		ResponseType response = Ignore;

		if(focused_widget_) {
			assign_key_press_event(focused_widget_, event);
		}

		return response;
	}

	ResponseType Frame::MousePressEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_frame(event);

		//DispatchMouseHoverEvent(event);

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
				SetFocused(dynamic_cast<Widget*>(widget));
			}
			custom_focused_widget_ = false;

		}

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType Frame::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_frame(event);

		//DispatchMouseHoverEvent(event);

		if(focused_widget_) {
			Point pos = focused_widget_->GetGlobalPosition();
			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - pos.x(),
					event.position().y() - pos.y());
			retval = assign_mouse_release_event(focused_widget_, event);
		}

		if(retval == Accept) return retval;

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType Frame::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		// TODO: make sure focused widget is still in this viewport
		/*
		if(focused_widget_) {
			Point pos = focused_widget_->GetGlobalPosition();
			const_cast<MouseEvent&>(event).set_local_position(event.position().x() - pos.x(), event.position().y() - pos.y());
			retval = assign_mouse_move_event(focused_widget_, event);
		}

		if(retval == Accept) {
			CheckAndUpdateHoverWidget(event);
			return retval;
		}
		*/

		if(cursor_followed_widget_) {

			set_event_frame(event);

			Point pos = this->GetGlobalPosition(cursor_followed_widget_);
			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - pos.x(),
					event.position().y() - pos.y());

			retval = assign_mouse_move_event(cursor_followed_widget_, event);
			return retval;
		}

		/*
		if(top_hovered_widget_) {

			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - position().x() - offset().x(),
					event.position().y() - position().y() - offset().y());

			retval = DispatchMouseMoveEvent(top_hovered_widget_, event);

			//retval = assign_mouse_move_event(top_hovered_widget_, event);
		}
		*/

		return retval;
	}

	void Frame::DispatchMouseHoverEventInSubs(const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).set_local_position(
				event.local_position().x () - top_hovered_widget_->position ().x ()
				- top_hovered_widget_->offset ().x (),
				event.local_position().y () - top_hovered_widget_->position ().y ()
				- top_hovered_widget_->offset ().y ()
		);

		for (AbstractWidget* p = top_hovered_widget_->last_child (); p;
				p = p->previous ()) {
			if (p->visiable () && p->Contain (event.local_position())) {

				top_hovered_widget_ = dynamic_cast<Widget*>(p);
				set_widget_mouse_hover_in_event (top_hovered_widget_, event);

				DispatchMouseHoverEventInSubs(event);
				break;
			}
		}
	}

	void Frame::OnFocusedWidgetDestroyed(Widget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Frame::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void Frame::OnHoverWidgetDestroyed(Widget* widget)
	{
		assert(widget->hover());
		assert(top_hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Frame::OnHoverWidgetDestroyed);

		top_hovered_widget_ = 0;
	}

	void Frame::OnCursorFollowedWidgetDestroyed(Widget* widget)
	{
		assert(cursor_followed_widget_ == widget);

		DBG_PRINT_MSG("cursor followed widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Frame::OnCursorFollowedWidgetDestroyed);

		cursor_followed_widget_ = 0;
	}

	void Frame::ClearHoverWidgets()
	{
		if(top_hovered_widget_) {

			top_hovered_widget_->destroyed().disconnectOne(this, &Frame::OnHoverWidgetDestroyed);

			while (top_hovered_widget_ && dynamic_cast<AbstractWidget*>(top_hovered_widget_) != this) {
				set_widget_hover_status(top_hovered_widget_, false);
				top_hovered_widget_ = dynamic_cast<Widget*>(top_hovered_widget_->parent());
			}

			if(dynamic_cast<AbstractWidget*>(top_hovered_widget_) == this)
				top_hovered_widget_ = 0;

		}

		if(cursor_followed_widget_) {
			cursor_followed_widget_->destroyed().disconnectOne(this, &Frame::OnCursorFollowedWidgetDestroyed);
			cursor_followed_widget_ = 0;
		}
	}

}
