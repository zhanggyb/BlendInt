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

#include <BlendInt/Frame.hpp>
#include <BlendInt/Types.hpp>
#include <BlendInt/Coord.hpp>
#include <BlendInt/Color.hpp>

#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	Frame::Frame ()
			: Widget()
	{
		set_minimal_size(margin().left() + margin().right(),
		        margin().top() + margin().bottom());
		set_size(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::Frame (AbstractWidget* parent)
			: Widget(parent)
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

	void Frame::SetMargin (const Margin& margin)
	{
		if (m_margin.equal(margin))
			return;

		Update(FrameMargin, &margin);
		m_margin = margin;
	}

	void Frame::SetMargin (int l, int r, int t, int b)
	{
		if (m_margin.equal(l, r, t, b))
			return;

		Margin new_margin(l, r, t, b);

		Update(FrameMargin, &new_margin);
		m_margin = new_margin;
	}

	void Frame::Draw ()
	{
		Widget::Draw();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		glDisable(GL_BLEND);
	}

	bool Frame::ContainNoMargin (const Coord2d& cursor)
	{
		if (cursor.x() < (position().x() + m_margin.left())
		        || cursor.y() < (position().y() + m_margin.bottom())
		        || cursor.x()
		                > (position().x() + size().width() - m_margin.right())
		        || cursor.y()
		                > (position().y() + size().height() - m_margin.top())) {
			return false;
		}

		return true;
	}

} /* namespace BlendInt */

