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

#include <math.h>
#include <algorithm>

#include <BlendInt/Gui/Clock.hpp>

namespace BlendInt {

	Clock::Clock()
	: AbstractWidget(), m_angle(0), m_timer(0)
	{
		Init();
	}

	Clock::~Clock ()
	{
	}

	ResponseType Clock::Draw(const RedrawEvent& event)
	{
//		int radius = std::min(size().width(), size().height()) / 2;
//
//		glColor4ub(m_background.red(), m_background.green(), m_background.blue(), m_background.alpha());
//		DrawCircle(size().width() / 2, size().height() / 2, radius, true);
//
//		glTranslatef(size().width() / 2, size().height() / 2, 0.0);
//		glRotatef(-m_angle, 0, 0, 1);
//
//		glEnable(GL_LINE_SMOOTH);
//
//		glLineWidth(2);
//		glColor3ub(255, 0, 0);
//		glBegin(GL_LINES);
//		glVertex2i(-5, 0);
//		glVertex2i(radius - 5, 0);
//		glEnd();
//
//		glDisable(GL_LINE_SMOOTH);
		return Ignore;
	}

	void Clock::UpdateClockHands()
	{
		m_angle = m_angle + 6;
		if(m_angle > 360) {
			m_angle = m_angle % 360;
		}

		Refresh();
	}

	bool Clock::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		return true;
	}

	void Clock::UpdateGeometry (const GeometryUpdateRequest& request)
	{
	}

	void Clock::BroadcastUpdate (const GeometryUpdateRequest& request)
	{
	}

	ResponseType Clock::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Clock::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Clock::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Clock::Init ()
	{
		//m_background.SetValue(45, 45, 45, 235);
		set_size(200, 200);
		//set_preferred_size(200, 200);

		m_timer.reset(new Timer);
		m_timer->SetInterval(1000);

		events()->connect(m_timer->timeout(), this, &Clock::UpdateClockHands);

		m_timer->Start();
	}

}
