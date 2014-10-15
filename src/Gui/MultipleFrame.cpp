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

#include <BlendInt/Gui/MultipleFrame.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	MultipleFrame::MultipleFrame()
	: Frame(),
	  focused_widget_(0),
	  hovered_widget_(0)
	{
		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);
	}

	MultipleFrame::~MultipleFrame()
	{
		if(focused_widget_) {
			set_widget_focus_status(focused_widget_, false);
			focused_widget_->destroyed().disconnectOne(this, &MultipleFrame::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		ClearHoverWidgets();
	}

	Widget* MultipleFrame::GetFocusedWidget() const
	{
		return focused_widget_;
	}

	Widget* MultipleFrame::GetHoveredWidget() const
	{
		return hovered_widget_;
	}

	void MultipleFrame::DispatchHoverEvent(const MouseEvent& event)
	{
		set_event_frame(event);

		// find the new top hovered widget
		if (hovered_widget_) {

			AbstractWidget* parent = hovered_widget_->parent();

			Point parent_position = parent->GetGlobalPosition();

			bool not_hover_through = event.position().x() < parent_position.x() ||
					event.position().y() < parent_position.y() ||
					event.position().x() > (parent_position.x() + parent->size().width()) ||
					event.position().y() > (parent_position.y() + parent->size().height());

			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - parent_position.x() - parent->offset().x(),
					event.position().y() - parent_position.y() - parent->offset().y());

			if(!not_hover_through) {

				if(hovered_widget_->Contain(event.local_position())) {

					Widget* orig = hovered_widget_;

					DispatchMouseHoverEventInSubs(event);

					if(orig != hovered_widget_) {
						orig->destroyed().disconnectOne(this,
								&MultipleFrame::OnHoverWidgetDestroyed);
						events()->connect(hovered_widget_->destroyed(), this,
						        &MultipleFrame::OnHoverWidgetDestroyed);
					}

				} else {

					hovered_widget_->destroyed ().disconnectOne (this,
								&MultipleFrame::OnHoverWidgetDestroyed);
					set_widget_mouse_hover_out_event(hovered_widget_, event);

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

					hovered_widget_ = dynamic_cast<Widget*>(parent);

					if(hovered_widget_) {
						DispatchMouseHoverEventInSubs(event);
						events()->connect(hovered_widget_->destroyed(), this,
						        &MultipleFrame::OnHoverWidgetDestroyed);
					}

				}

			} else {

				hovered_widget_->destroyed().disconnectOne(this,
					        &MultipleFrame::OnHoverWidgetDestroyed);
				set_widget_mouse_hover_out_event(hovered_widget_, event);

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

				hovered_widget_ = dynamic_cast<Widget*>(parent);
				if(hovered_widget_) {
					DispatchMouseHoverEventInSubs(event);
					events()->connect(hovered_widget_->destroyed(), this,
					        &MultipleFrame::OnHoverWidgetDestroyed);
				}

			}

		} else {

			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - position().x() - offset().x(),
					event.position().y() - position().y() - offset().y());

			for(AbstractWidget* p = last_child(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(event.local_position())) {

					hovered_widget_ = dynamic_cast<Widget*>(p);
					set_widget_mouse_hover_in_event(hovered_widget_, event);

					break;
				}
			}

			if(hovered_widget_) {
				DispatchMouseHoverEventInSubs(event);
				events()->connect(hovered_widget_->destroyed(), this,
				        &MultipleFrame::OnHoverWidgetDestroyed);
			}

		}

	}

	void MultipleFrame::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			float x = static_cast<float>(request.position()->x() + offset().x());
			float y = static_cast<float>(request.position()->y() + offset().y());

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

	void MultipleFrame::PerformSizeUpdate(const SizeUpdateRequest& request)
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
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void MultipleFrame::PreDraw(Profile& profile)
	{
		assign_profile_frame(profile);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
		Shaders::instance->SetWidgetModelMatrix(model_matrix_);
	}

	ResponseType MultipleFrame::Draw(Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return subs_count() ? Ignore : Accept;
	}

	void MultipleFrame::PostDraw(Profile& profile)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, profile.context()->size().width(), profile.context()->size().height());
	}

	void MultipleFrame::DispatchMouseHoverEventInSubs(const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).set_local_position(
				event.local_position().x () - hovered_widget_->position ().x ()
				- hovered_widget_->offset ().x (),
				event.local_position().y () - hovered_widget_->position ().y ()
				- hovered_widget_->offset ().y ()
		);

		for (AbstractWidget* p = hovered_widget_->last_child (); p;
				p = p->previous ()) {
			if (p->visiable () && p->Contain (event.local_position())) {

				hovered_widget_ = dynamic_cast<Widget*>(p);
				set_widget_mouse_hover_in_event (hovered_widget_, event);

				DispatchMouseHoverEventInSubs(event);
				break;
			}
		}
	}

	void MultipleFrame::OnFocusedWidgetDestroyed(Widget* widget)
	{
		assert(focused_widget_ == widget);
		assert(widget->focus());

		//set_widget_focus_status(widget, false);
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &MultipleFrame::OnFocusedWidgetDestroyed);

		focused_widget_ = 0;
	}

	void MultipleFrame::OnHoverWidgetDestroyed(Widget* widget)
	{
		assert(widget->hover());
		assert(hovered_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &MultipleFrame::OnHoverWidgetDestroyed);

		hovered_widget_ = 0;
	}

	void MultipleFrame::MouseHoverOutEvent(const MouseEvent& event)
	{
		ClearHoverWidgets();
	}

	ResponseType MultipleFrame::KeyPressEvent(const KeyEvent& event)
	{
		set_event_frame(event);

		ResponseType response = Ignore;

		if(focused_widget_) {
			call_key_press_event(focused_widget_, event);
		}

		return response;
}

	ResponseType MultipleFrame::MousePressEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_frame(event);

		//DispatchMouseHoverEvent(event);

		if(hovered_widget_) {

			/*
			AbstractWidget* widget = 0;	// widget may be focused

			custom_focused_widget_ = false;

			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - position().x() - offset().x(),
					event.position().y() - position().y() - offset().y());

			retval = DispatchMousePressEvent(hovered_widget_, event);

			if(retval == Accept) {
				widget = hovered_widget_;
			}

			if(!custom_focused_widget_) {
				SetFocused(dynamic_cast<Widget*>(widget));
			}
			custom_focused_widget_ = false;

			 */
		}

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType MultipleFrame::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		set_event_frame(event);

		//DispatchMouseHoverEvent(event);

		if(focused_widget_) {
			Point pos = focused_widget_->GetGlobalPosition();
			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - pos.x(),
					event.position().y() - pos.y());
			retval = call_mouse_release_event(focused_widget_, event);
		}

		if(retval == Accept) return retval;

		if(display_mode() == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType MultipleFrame::MouseMoveEvent(const MouseEvent& event)
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

		/*
		if(cursor_followed_widget_) {

			set_event_frame(event);

			Point pos = this->GetGlobalPosition(cursor_followed_widget_);
			const_cast<MouseEvent&>(event).set_local_position(
					event.position().x() - pos.x(),
					event.position().y() - pos.y());

			retval = call_mouse_move_event(cursor_followed_widget_, event);
			return retval;
		}
		*/

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

	void MultipleFrame::ClearHoverWidgets()
	{
		if(hovered_widget_) {

			hovered_widget_->destroyed().disconnectOne(this, &MultipleFrame::OnHoverWidgetDestroyed);

			while (hovered_widget_ && dynamic_cast<AbstractWidget*>(hovered_widget_) != this) {
				set_widget_hover_status(hovered_widget_, false);
				hovered_widget_ = dynamic_cast<Widget*>(hovered_widget_->parent());
			}

			if(dynamic_cast<AbstractWidget*>(hovered_widget_) == this)
				hovered_widget_ = 0;

		}

		/*
		if(cursor_followed_widget_) {
			cursor_followed_widget_->destroyed().disconnectOne(this, &Frame::OnCursorFollowedWidgetDestroyed);
			cursor_followed_widget_ = 0;
		}
		*/
	}

}
