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
#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	Widget::Widget()
	: AbstractWidget()
	{
		set_size(120, 80);
	}

	Widget::~Widget()
	{

	}

	bool Widget::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		return true;
	}

	void Widget::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		// do nothing in this base class
		ReportGeometryUpdate(request);
	}

	void Widget::BroadcastUpdate(const GeometryUpdateRequest& request)
	{
		// do nothing
	}

	ResponseType Widget::Draw(const RedrawEvent& event)
	{
		return Ignore;
	}

	ResponseType Widget::CursorEnterEvent(bool entered)
	{
		return Accept;
	}

	ResponseType Widget::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}
	
	ResponseType Widget::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Widget::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

}

