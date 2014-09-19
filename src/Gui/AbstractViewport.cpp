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

#include <BlendInt/Gui/AbstractViewport.hpp>

namespace BlendInt {

	glm::mat4 AbstractViewport::default_view_matrix = glm::lookAt(
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f, 0.f, 0.f),
	         	glm::vec3(0.f, 1.f, 0.f));

	AbstractViewport::AbstractViewport()
	: AbstractContainer(),
	  focused_(0),
	  top_hovered_(0),
	  display_mode_(Normal),
	  custom_focused_widget_(false)
	{
	}

	AbstractViewport::~AbstractViewport()
	{
	}

	void AbstractViewport::SetFocused(AbstractWidget* widget)
	{
		custom_focused_widget_ = true;

		if(focused_ == widget)
			return;

		if (focused_) {
			focused_->set_focus(false);
			focused_->destroyed().disconnectOne(this, &AbstractViewport::OnFocusedWidgetDestroyed);
			focused_->FocusEvent(false);
		}

		focused_ = widget;
		if (focused_) {
			focused_->set_focus(true);
			events()->connect(focused_->destroyed(), this, &AbstractViewport::OnFocusedWidgetDestroyed);
			focused_->FocusEvent(true);
		}
	}

	AbstractViewport* AbstractViewport::GetViewport(AbstractWidget* widget)
	{
		AbstractContainer* container = widget->container();
		AbstractWidget* viewport = 0;

		if(container == 0) {
			return dynamic_cast<AbstractViewport*>(widget);
		} else {

			while(container->container()) {
				viewport = container;
				container = container->container();
			}

		}

		return dynamic_cast<AbstractViewport*>(viewport);
	}

	bool AbstractViewport::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		return true;
	}

	bool AbstractViewport::PositionUpdateTest(
			const PositionUpdateRequest& request)
	{
		return true;
	}

	bool AbstractViewport::RoundTypeUpdateTest(
			const RoundTypeUpdateRequest& request)
	{
		return true;
	}

	bool AbstractViewport::RoundRadiusUpdateTest(
			const RoundRadiusUpdateRequest& request)
	{
		return true;
	}

	void AbstractViewport::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
	}

	void AbstractViewport::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
	}

	void AbstractViewport::PerformRoundTypeUpdate(
			const RoundTypeUpdateRequest& request)
	{
	}

	void AbstractViewport::PerformRoundRadiusUpdate(
			const RoundRadiusUpdateRequest& request)
	{
	}

	ResponseType AbstractViewport::FocusEvent(bool focus)
	{
		return Ignore;
	}

	ResponseType AbstractViewport::CursorEnterEvent(bool entered)
	{
		return Ignore;
	}

	ResponseType AbstractViewport::KeyPressEvent(const KeyEvent& event)
	{
		const_cast<KeyEvent&>(event).viewport_ = this;

		ResponseType response = Ignore;

		if(focused_) {
			response = focused_->KeyPressEvent(event);
		}

		return response;
	}

	ResponseType AbstractViewport::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractViewport::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractViewport::MousePressEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		const_cast<MouseEvent&>(event).viewport_ = this;

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

		if(display_mode_ == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType AbstractViewport::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		const_cast<MouseEvent&>(event).viewport_ = this;

		CheckAndUpdateHoverWidget(event);

		if(focused_) {
			retval = focused_->MouseReleaseEvent(event);
		}

		if(retval == Accept) return retval;

		if(display_mode_ == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType AbstractViewport::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		const_cast<MouseEvent&>(event).viewport_ = this;

		if(Contain(event.global_position())) {
			const_cast<MouseEvent&>(event).set_local_position(
					event.global_position().x() - position().x(),
					event.global_position().y() - position().y());
		} else {
			return Ignore;
		}

		CheckAndUpdateHoverWidget(event);

		if(top_hovered_) {
			retval = top_hovered_->MouseMoveEvent(event);
		}

		if(display_mode_ == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType AbstractViewport::DispatchMousePressEvent(
			AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMousePressEvent(widget->container(), event) == Ignore) {
					return widget->MousePressEvent(event);
				} else {
					return Accept;
				}

			} else {
				return widget->MousePressEvent(event);
			}

		}
	}

	ResponseType AbstractViewport::DispatchMouseReleaseEvent(
			AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMouseReleaseEvent(widget->container(), event) == Ignore) {
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

	bool AbstractViewport::CheckAndUpdateHoverWidget(const MouseEvent& event)
	{
		if (top_hovered_) {

			if (IsHoverThroughExt (top_hovered_->container_,
					event.global_position ())) {

				if (top_hovered_->Contain (event.local_position ())) {

					AbstractWidget* orig = top_hovered_;
					UpdateHoverWidgetSubs (event);

					if (orig != top_hovered_) {
						orig->destroyed ().disconnectOne (this,
								&AbstractViewport::OnHoverWidgetDestroyed);
						events ()->connect (top_hovered_->destroyed (),
								this, &AbstractViewport::OnHoverWidgetDestroyed);
					}

				} else {

					top_hovered_->destroyed ().disconnectOne (this,
							&AbstractViewport::OnHoverWidgetDestroyed);
					top_hovered_->set_hover (false);
					top_hovered_->CursorEnterEvent (false);

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
								this, &AbstractViewport::OnHoverWidgetDestroyed);
					}
				}

			} else {

				top_hovered_->destroyed().disconnectOne(this,
				        &AbstractViewport::OnHoverWidgetDestroyed);
				top_hovered_->set_hover(false);
				top_hovered_->CursorEnterEvent(false);

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
					events()->connect(top_hovered_->destroyed(), this, &AbstractViewport::OnHoverWidgetDestroyed);
				}

			}

		} else {

			for(AbstractWidget* p = last(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(event.local_position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					top_hovered_ = p;
					top_hovered_->set_hover(true);
					top_hovered_->CursorEnterEvent(true);

					break;
				}
			}

			if(top_hovered_) {

				UpdateHoverWidgetSubs(event);

				events()->connect(top_hovered_->destroyed(), this,
				        &AbstractViewport::OnHoverWidgetDestroyed);
			}

		}

		return top_hovered_ != 0;
	}

	void AbstractViewport::UpdateHoverWidgetSubs(const MouseEvent& event)
	{
		AbstractContainer* parent = dynamic_cast<AbstractContainer*>(top_hovered_);

		if (parent) {

			for(AbstractWidget* p = parent->last(); p; p = p->previous())
			{
				if(p->visiable() && p->Contain(event.local_position())) {

					top_hovered_ = p;
					top_hovered_->set_hover(true);
					top_hovered_->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}

		}
	}

	void AbstractViewport::OnFocusedWidgetDestroyed(AbstractWidget* widget)
	{
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());

		assert(focused_ == widget);

		if(widget->focused()) {
			widget->set_focus(false);
			widget->destroyed().disconnectOne(this, &AbstractViewport::OnFocusedWidgetDestroyed);
		}

		focused_ = 0;
	}

	void AbstractViewport::OnHoverWidgetDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(top_hovered_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &AbstractViewport::OnHoverWidgetDestroyed);

		top_hovered_ = 0;
	}

	void AbstractViewport::ClearHoverWidgets()
	{
		if(top_hovered_) {

			top_hovered_->destroyed().disconnectOne(this, &AbstractViewport::OnHoverWidgetDestroyed);

			while (top_hovered_ && top_hovered_ != this) {
				top_hovered_->set_hover(false);
				top_hovered_ = top_hovered_->container();
			}

			if(top_hovered_ == this)
				top_hovered_ = 0;

		}
	}

}
