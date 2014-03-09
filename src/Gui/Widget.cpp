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

#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ContextManager.hpp>

namespace BlendInt {

	Widget::Widget()
	: AbstractWidget()
	{
		set_minimal_size(0, 0);
		set_size(120, 80);
		set_preferred_size(120, 80);

		ContextManager::instance->AddSubWidget(this);
	}

	Widget::~Widget()
	{

	}

	bool Widget::Update (int type, const void* data)
	{
		// do nothing in this base class
		return true;
	}

	void Widget::Draw(RedrawEvent* event)
	{
		float outer_v[4][2];	// vertices for drawing outline
		float inner_v[4][2];	// vertices for drawing inner

		generate_rect_vertices(&(size()), DefaultBorderWidth(), inner_v, outer_v);

		float quad_strip[4 * 2 + 2][2]; // + 2 because the last pair is wrapped

		verts_to_quad_strip (inner_v, outer_v, 4, quad_strip);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());

		DrawInnerArray(inner_v, 4);

		// draw outline
		unsigned char tcol[4] = {themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		DrawOutlineArray(quad_strip, 4 * 2 + 2);
	}

	void Widget::CursorEnterEvent(bool entered)
	{

	}

	void Widget::KeyPressEvent (KeyEvent* event)
	{
	}

	void Widget::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void Widget::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void Widget::MousePressEvent (MouseEvent* event)
	{
	}

	void Widget::MouseReleaseEvent (MouseEvent* event)
	{
	}

	void Widget::MouseMoveEvent (MouseEvent* event)
	{
	}

}

