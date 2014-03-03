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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	Frame::Frame ()
			: AbstractContainer()
	{
		set_minimal_size(margin().left() + margin().right(),
		        margin().top() + margin().bottom());
		set_size(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::~Frame ()
	{
		// TODO Auto-generated destructor stub
	}


	bool Frame::Update (int type, const void* data)
	{
		return true;
	}

	void Frame::CursorEnterEvent (bool entered)
	{
	}

	void Frame::KeyPressEvent (KeyEvent* event)
	{
	}

	void Frame::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void Frame::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void Frame::MousePressEvent (MouseEvent* event)
	{
	}

	void Frame::MouseReleaseEvent (MouseEvent* event)
	{
	}

	void Frame::MouseMoveEvent (MouseEvent* event)
	{
	}

	void Frame::Draw ()
	{
#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 0.0f, 0.0f, 0.45f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(size().width(), 0);
		glVertex2i(size().width(), size().height());
		glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif
	}

} /* namespace BlendInt */

