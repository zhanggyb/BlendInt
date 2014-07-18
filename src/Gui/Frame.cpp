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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	Frame::Frame ()
	: AbstractContainer(1)
	{
		set_size(400, 300);
	}

	Frame::~Frame ()
	{
	}

	bool Frame::Setup (AbstractWidget* widget)
	{
		bool ret = false;

		if (AssignSubWidget(0, widget)) {

			FillSingleWidget(0, position(), size(), margin());

			ret = true;
		}

		return ret;
	}

	bool Frame::Remove (AbstractWidget* widget)
	{
		bool ret = false;

		if(RemoveSubWidget(widget)) {

			ret = true;
		}

		return ret;
	}

	bool Frame::IsExpandX() const
	{
		if(deque().size())
			return deque()[0]->IsExpandX();
		else
			return false;
	}

	bool Frame::IsExpandY() const
	{
		if(deque().size())
			return deque()[0]->IsExpandY();
		else
			return false;
	}

	Size Frame::GetPreferredSize() const
	{
		Size prefer(400, 300);

		AbstractWidget* widget = deque()[0];

		if(widget) {
			prefer = widget->GetPreferredSize();

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void Frame::PerformMarginUpdate(const Margin& request)
	{
		if(deque().size()) {
			set_margin(request);

			//FillSubWidget(position(), size(), request);
		}
	}

	bool Frame::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	bool Frame::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	void Frame::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());

			if (deque().size()) {
				//FillSubWidget(position(), *request.size(), margin());
			}
		}

		ReportSizeUpdate(request);
	}

	void Frame::PerformPositionUpdate (
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			set_position(*request.position());
			SetSubWidgetPosition(deque()[0],
					request.position()->x() + margin().left(),
					request.position()->y() + margin().bottom());
		}

		ReportPositionUpdate(request);
	}

	ResponseType Frame::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType Frame::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::Draw (const RedrawEvent& event)
	{
		return Ignore;
	}

} /* namespace BlendInt */
