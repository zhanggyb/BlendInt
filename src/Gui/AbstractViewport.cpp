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
	  display_mode_(Normal)
	{
	}

	AbstractViewport::~AbstractViewport()
	{
	}

	AbstractViewport* AbstractViewport::GetViewport(AbstractWidget* widget)
	{
		AbstractContainer* container = widget->container();
		AbstractViewport* viewport = 0;

		if(container == 0) {
			return dynamic_cast<AbstractViewport*>(widget);
		} else {
			while(container) {
				viewport = dynamic_cast<AbstractViewport*>(container);
				if(viewport) break;
				container = container->container();
			}
		}

		return viewport;
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

	ResponseType AbstractViewport::DispatchMousePressEvent(
			AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMousePressEvent(widget->container(), event) == Ignore) {
					const_cast<MouseEvent&>(event).set_local_position(
							event.local_position().x() - widget->position().x() - widget->container()->offset_x(),
							event.local_position().y() - widget->position().y() - widget->container()->offset_y());
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

}
