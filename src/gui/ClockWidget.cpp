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

#include <BlendInt/ClockWidget.hpp>

namespace BlendInt {

	ClockWidget::ClockWidget()
	: Widget(), m_angle(0), m_timer(0)
	{
		Init();
	}

	ClockWidget::ClockWidget(AbstractWidget* parent)
	: Widget (parent), m_angle(0), m_timer(0)
	{
		Init();
	}

	ClockWidget::~ClockWidget ()
	{
		Destroy(m_timer);
	}

	void ClockWidget::Draw()
	{
		int radius = std::min(size().width(), size().height()) / 2;

		glColor4ub(m_background.red(), m_background.green(), m_background.blue(), m_background.alpha());
		DrawCircle(size().width() / 2, size().height() / 2, radius, true);

		glTranslatef(size().width() / 2, size().height() / 2, 0.0);
		glRotatef(-m_angle, 0, 0, 1);

		glEnable(GL_LINE_SMOOTH);

		glLineWidth(2);
		glColor3ub(255, 0, 0);
		glBegin(GL_LINES);
		glVertex2i(-5, 0);
		glVertex2i(radius - 5, 0);
		glEnd();

		glDisable(GL_LINE_SMOOTH);
	}

	void ClockWidget::UpdateClockHands()
	{
		m_angle = m_angle + 6;
		if(m_angle > 360) {
			m_angle = m_angle % 360;
		}
	}

	void ClockWidget::DrawArc (float x, float y, float start_angle,
	        float end_angle, float delta_angle, float radius, bool fill)
	{
		if (fill) {
			glBegin(GL_TRIANGLE_FAN);
		} else {
			glBegin(GL_LINE_STRIP);
		}

		float vx, vy;

		for (float i = start_angle; i <= end_angle; i += delta_angle) {
			vx = x + radius * cos(i);
			vy = y + radius * sin(i);
			glVertex2d(vx, vy);
		}
		glEnd();
	}

	void ClockWidget::DrawCircle (float x, float y, float radius, bool fill)
	{
		float pi = acos(-1.0);
		DrawArc(x, y, 0, 2 * pi, pi / 180, radius, fill);
	}

	void ClockWidget::DrawPie (float x, float y, float start_angle,
	        float end_angle, float delta_angle, float radius, bool fill)
	{
		DrawArc(x, y, start_angle, end_angle, delta_angle, radius, fill);
		if (fill) {
			glBegin(GL_TRIANGLE_FAN);
		} else {
			glBegin(GL_LINES);
		}
		glVertex2d(x, y);
		glVertex2d(x + radius * cos(start_angle),
		        y + radius * sin(start_angle));
		if (!fill) {
			glVertex2d(x, y);
		}
		glVertex2d(x + radius * cos(end_angle), y + radius * sin(end_angle));

		glEnd();
	}

	void ClockWidget::Init ()
	{
		m_background.set_color(45, 45, 45, 235);
		set_size(200, 200);
		set_preferred_size(200, 200);
		set_expand_x(true);
		set_expand_y(true);

		m_timer = new Timer;
		Retain(m_timer);
		m_timer->SetInterval(1000);

		events()->connect(m_timer->timeout(), this, &ClockWidget::UpdateClockHands);

		m_timer->Start();
	}

}
