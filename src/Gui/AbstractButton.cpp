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

#include <BlendInt/Gui/AbstractButton.hpp>

#include <iostream>

namespace BlendInt {

	Margin AbstractButton::default_button_padding = Margin(2, 2, 2, 2);

	AbstractButton::AbstractButton () :
			Widget()
	{
	}

	AbstractButton::~AbstractButton ()
	{

	}

	ResponseType AbstractButton::CursorEnterEvent(bool entered)
	{
		if(entered) {

			if(m_status[ButtonPressed]) {
				m_status[ButtonDown] = 1;

				if(m_status[ButtonCheckable]) {
					m_status[ButtonChecked] = !m_status[ButtonChecked];
				}
			}

			Refresh();
		} else {

			if(m_status[ButtonPressed]) {
				m_status[ButtonDown] = 0;

				if(m_status[ButtonCheckable]) {
					m_status[ButtonChecked] = !m_status[ButtonChecked];
				}

			}

			Refresh();
		}

		return Accept;
	}

	ResponseType AbstractButton::MousePressEvent (const MouseEvent& event)
	{
		if (event.button() == MouseButtonLeft) {
			m_status.set(ButtonPressed);
			m_status.set(ButtonDown);

			if (m_status[ButtonCheckable]) {
				m_status[ButtonLastChecked] = m_status[ButtonChecked];
				m_status[ButtonChecked] = !m_status[ButtonChecked];
			}

			Refresh();

			m_pressed.fire();
			return Accept;
		}

		return Ignore;
	}

	ResponseType AbstractButton::MouseReleaseEvent(const MouseEvent& event)
	{
		if (event.button() == MouseButtonLeft) {
			int fire_event = 0;	// 0: no event, 1: click event, 2: toggled event

			if (m_status[ButtonCheckable]) {
				if (m_status[ButtonPressed]) {
					fire_event = 2;
				}
			} else {
				if (m_status[ButtonPressed] && m_status[ButtonDown]) {
					fire_event = 1;
				}
			}

			Refresh();

			switch (fire_event) {

				case 0:
					break;

				case 1:
					m_clicked.fire();
					break;

				case 2: {
					if (m_status[ButtonChecked]
									!= m_status[ButtonLastChecked]) {
						m_toggled.fire(m_status[ButtonChecked]);
					}
					break;
				}

				default:
					break;
			}

			m_status.reset(ButtonPressed);
			m_status.reset(ButtonDown);

			m_released.fire();

			return Accept;
		}

		return Ignore;
	}

	ResponseType AbstractButton::MouseMoveEvent (const MouseEvent& event)
	{
		/*
		if (m_status[ButtonDown]) {
			event->accept(this);
			return;
		}
		*/
		return Accept;
	}
	
	void AbstractButton::SetDown (bool down)
	{
		if(m_status[ButtonCheckable]) {
			if(m_status[ButtonChecked] != down)
				Refresh();

			m_status[ButtonChecked] = down ? 1 : 0;
		} else {

			if(m_status[ButtonDown] != down)
				Refresh();

			m_status[ButtonDown] = down ? 1 : 0;
		}
	}

	void AbstractButton::SetCheckable (bool checkable)
	{
		if(!checkable) {
			m_status[ButtonChecked] = false;
		}

		m_status[ButtonCheckable] = checkable ? 1 : 0;
	}

	void AbstractButton::SetChecked (bool checked)
	{
		if(m_status[ButtonCheckable]) {

			if(m_status[ButtonChecked] == checked)
				return;

			m_status[ButtonChecked] = checked ? 1 : 0;
			Refresh();

			m_toggled.fire(m_status[ButtonChecked]);
		}
	}

} /* namespace BlendInt */
