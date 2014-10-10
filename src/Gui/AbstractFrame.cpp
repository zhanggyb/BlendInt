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

	void AbstractFrame::SetCursorFollowedWidget(AbstractWidget* widget)
	{

	}

	Point AbstractFrame::GetGlobalPosition (const Widget* widget)
	{
#ifdef DEBUG
		assert(widget);
#endif

		Point pos = widget->position();

		AbstractWidget* p = widget->parent();
		while(p && (p != this)) {
			pos.reset(
					pos.x() + p->position().x() + p->offset().x(),
					pos.y() + p->position().y() + p->offset().y());
			p = p->parent();
		}

		pos.reset(pos.x() + position().x() + offset().x(), pos.y() + position().y() + offset().y());

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

	ResponseType AbstractFrame::DispatchMousePressEvent(
			AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->parent ()) {
				if(DispatchMousePressEvent(widget->parent (), event) == Ignore) {
					const_cast<MouseEvent&>(event).set_local_position(
							event.local_position().x() - widget->position().x() - widget->parent ()->offset().x(),
							event.local_position().y() - widget->position().y() - widget->parent ()->offset().y());
					return widget->MousePressEvent(event);
				} else {
					return Accept;
				}

			} else {
				return widget->MousePressEvent(event);
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
					const_cast<MouseEvent&>(event).set_local_position(
							event.local_position().x() - widget->position().x() - widget->parent ()->offset().x(),
							event.local_position().y() - widget->position().y() - widget->parent ()->offset().y());
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

}
