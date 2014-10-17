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

#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	glm::mat4 AbstractFrame::default_view_matrix = glm::lookAt(
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));

	AbstractFrame::AbstractFrame()
	: AbstractWidget(),
	  display_mode_(Normal)
	{
		destroyed_.reset(new Cpp::Event<AbstractFrame*>);
	}

	AbstractFrame::~AbstractFrame()
	{
		destroyed_->fire(this);
	}

	Point AbstractFrame::GetAbsolutePosition (const Widget* widget)
	{
#ifdef DEBUG
		assert(widget);
#endif

		Point pos = widget->position();

		AbstractWidget* p = widget->parent();
		while(p && (p != this)) {
			pos = pos + p->position() + p->offset();
			p = p->parent();
		}

		pos = pos + position() + offset();
		return pos;
	}

	AbstractFrame* AbstractFrame::GetFrame(AbstractWidget* widget)
	{
		AbstractWidget* container = widget->parent ();
		AbstractFrame* frame = 0;

		if(container == 0) {
			return dynamic_cast<AbstractFrame*>(widget);
		} else {
			while(container) {
				frame = dynamic_cast<AbstractFrame*>(container);
				if(frame) break;
				container = container->parent ();
			}
		}

		return frame;
	}

	bool AbstractFrame::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		return true;
	}

	bool AbstractFrame::PositionUpdateTest(
			const PositionUpdateRequest& request)
	{
		return true;
	}

	bool AbstractFrame::RoundTypeUpdateTest(
			const RoundTypeUpdateRequest& request)
	{
		return true;
	}

	bool AbstractFrame::RoundRadiusUpdateTest(
			const RoundRadiusUpdateRequest& request)
	{
		return true;
	}

	void AbstractFrame::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
	}

	void AbstractFrame::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
	}

	void AbstractFrame::PerformRoundTypeUpdate(
			const RoundTypeUpdateRequest& request)
	{
	}

	void AbstractFrame::PerformRoundRadiusUpdate(
			const RoundRadiusUpdateRequest& request)
	{
	}

	ResponseType AbstractFrame::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType AbstractFrame::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	AbstractWidget* AbstractFrame::DispatchMousePressEvent(
			AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return 0;
		} else {

			ResponseType response = Ignore;
			AbstractWidget* ret_val = 0;

			if(widget->parent ()) {

				ret_val = DispatchMousePressEvent(widget->parent(), event);

				if(ret_val == 0) {

					response = widget->MousePressEvent(event);

					return response == Accept ? widget : 0;

				} else {
					return ret_val;
				}

			} else {
				response = widget->MousePressEvent(event);
				return response == Accept ? widget : 0;
			}

		}
	}

	ResponseType AbstractFrame::DispatchMouseMoveEvent(AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->parent ()) {
				if(DispatchMouseMoveEvent(widget->parent (), event) == Ignore) {
					return widget->MouseMoveEvent(event);
				} else {
					return Accept;
				}

			} else {
				return widget->MouseMoveEvent(event);
			}

		}
	}

	ResponseType AbstractFrame::DispatchMouseReleaseEvent(
			AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->parent ()) {
				if(DispatchMouseReleaseEvent(widget->parent (), event) == Ignore) {
					return widget->MouseReleaseEvent(event);
				} else {
					return Accept;
				}

			} else {
				DBG_PRINT_MSG("mouse press in %s", widget->name().c_str());
				return widget->MouseReleaseEvent(event);
			}

		}
	}

	Widget* AbstractFrame::DispatchHoverEventsInSubWidgets(Widget* orig,
			const MouseEvent& event)
	{
		Widget* hovered_widget = orig;

		set_event_frame(event);
		Point local_position;	// the relative local position of the cursor in a widget

		// find the new top hovered widget
		if (hovered_widget) {

			AbstractWidget* parent = hovered_widget->parent();

			Point parent_position = parent->GetGlobalPosition();

			bool not_hover_through = event.position().x() < parent_position.x() ||
					event.position().y() < parent_position.y() ||
					event.position().x() > (parent_position.x() + parent->size().width()) ||
					event.position().y() > (parent_position.y() + parent->size().height());

			local_position.reset(event.position().x() - parent_position.x() - parent->offset().x(),
					event.position().y() - parent_position.y() - parent->offset().y());

			if(!not_hover_through) {

				if(hovered_widget->Contain(local_position)) {

					Widget* orig = hovered_widget;

					hovered_widget = DispatchHoverEventDeeper(hovered_widget, event, local_position);

					if(orig != hovered_widget) {
//						orig->destroyed().disconnectOne(this,
//								&SingleFrame::OnHoverWidgetDestroyed);
//						events()->connect(hovered_widget->destroyed(), this,
//						        &SingleFrame::OnHoverWidgetDestroyed);
					}

				} else {

//					hovered_widget->destroyed ().disconnectOne (this,
//								&SingleFrame::OnHoverWidgetDestroyed);
					set_widget_mouse_hover_out_event(hovered_widget, event);
//					hovered_widget->set_hover(false);
//					hovered_widget->MouseHoverOutEvent(event);


					// find which contianer contains cursor position
					while (parent) {

						if (parent == this) {	// FIXME: the widget may be mvoed to another context
							parent = 0;
							break;
						}

						local_position.reset(
								local_position.x() + parent->position().x() + parent->offset().x(),
								local_position.y() + parent->position().y() + parent->offset().y());

						if (parent->Contain(local_position)) break;

						parent = parent->parent();
					}

					hovered_widget = dynamic_cast<Widget*>(parent);

					if(hovered_widget) {
						hovered_widget = DispatchHoverEventDeeper(hovered_widget, event, local_position);
//						events()->connect(hovered_widget->destroyed(), this,
//						        &SingleFrame::OnHoverWidgetDestroyed);
					}

				}

			} else {

//				hovered_widget->destroyed().disconnectOne(this,
//					        &SingleFrame::OnHoverWidgetDestroyed);
				set_widget_mouse_hover_out_event(hovered_widget, event);
//				hovered_widget->set_hover(false);
//				hovered_widget->MouseHoverOutEvent(event);


				// find which contianer contains cursor position
				parent = parent->parent();
				while (parent) {

					if (parent == this) {	// FIXME: the widget may be mvoed to another context
						parent = 0;
						break;
					}

					local_position.reset(
							local_position.x() + parent->position().x() + parent->offset().x(),
							local_position.y() + parent->position().y() + parent->offset().y());

					if(IsHoverThroughExt(parent, event.position())) break;
					parent = parent->parent();
				}

				hovered_widget = dynamic_cast<Widget*>(parent);
				if(hovered_widget) {
					hovered_widget = DispatchHoverEventDeeper(hovered_widget, event, local_position);
//					events()->connect(hovered_widget->destroyed(), this,
//					        &SingleFrame::OnHoverWidgetDestroyed);
				}

			}

		} else {

			local_position.reset(
					event.position().x() - position().x() - offset().x(),
					event.position().y() - position().y() - offset().y());

			for(AbstractWidget* p = last_child(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(local_position)) {

					hovered_widget = dynamic_cast<Widget*>(p);
					set_widget_mouse_hover_in_event(hovered_widget, event);
//					hovered_widget->set_hover(true);
//					hovered_widget->MouseHoverInEvent(event);

					break;
				}
			}

			if(hovered_widget) {
				hovered_widget = DispatchHoverEventDeeper(hovered_widget, event, local_position);
//				events()->connect(hovered_widget->destroyed(), this,
//				        &SingleFrame::OnHoverWidgetDestroyed);
			}

		}

		return hovered_widget;
	}

	void AbstractFrame::ClearHoverWidgets(Widget* hovered_widget)
	{
		assert(hovered_widget);

		while (hovered_widget && dynamic_cast<AbstractWidget*>(hovered_widget) != this) {
			set_widget_hover_status(hovered_widget, false);
			hovered_widget = dynamic_cast<Widget*>(hovered_widget->parent());
		}

		if(dynamic_cast<AbstractWidget*>(hovered_widget) == this)
			hovered_widget = 0;
	}

	Widget* AbstractFrame::DispatchHoverEventDeeper(Widget* widget, const MouseEvent& event,
			Point& local_position)
	{
		Widget* retval = widget;

		local_position.reset(
				local_position.x () - widget->position ().x ()
				- widget->offset ().x (),
				local_position.y () - widget->position ().y ()
				- widget->offset ().y ()
		);

		for (AbstractWidget* p = widget->last_child (); p;
				p = p->previous ()) {

			if (p->visiable () && p->Contain (local_position)) {
				retval = dynamic_cast<Widget*>(p);
				set_widget_mouse_hover_in_event (retval, event);

				retval = DispatchHoverEventDeeper(retval, event, local_position);
				break;
			}
	
		}

		return retval;
	}

}
