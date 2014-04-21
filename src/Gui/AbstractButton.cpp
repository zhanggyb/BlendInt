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

	AbstractButton::AbstractButton () :
			Widget(),
			m_text_length(0)
	{
	}

	AbstractButton::~AbstractButton ()
	{

	}

	void AbstractButton::SetText (const String& text)
	{
		if(text.empty()) {
			Resize (90, 20);
			return;
		}

		bool cal_width = true;

		m_text = text;

		m_text_outline = m_font.get_text_outline(m_text);

		m_text_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				Resize(size().width(), m_text_outline.height());
			} else {
				m_text_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				Resize(m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_text_length = GetValidTextSize();
			}
		}

		// FIXME: the alignment and origin was set in Resize -> Update, reset here?
		m_origin.set_x((size().width() - m_text_outline.width()) / 2);

		m_origin.set_y((size().height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
	}

	void AbstractButton::SetFont (const Font& font)
	{
		bool cal_width = true;

		m_font = font;

		m_text_outline = m_font.get_text_outline(m_text);

		m_text_length = m_text.length();

		if(size().height() < m_text_outline.height()) {
			if(expand_y()) {
				Resize(size().width(), m_text_outline.height());
			} else {
				m_text_length = 0;
				cal_width = false;
			}
		}

		if(size().width() < m_text_outline.width()) {
			if(expand_x()) {
				Resize(m_text_outline.width(), size().height());
			} else {
				if(cal_width) m_text_length = GetValidTextSize();
			}
		}

		// FIXME: the alignment and origin was set in Resize -> Update, reset here?

		m_origin.set_x((size().width() - m_text_outline.width()) / 2);

		m_origin.set_y((size().height() - m_font.get_height()) / 2 + std::abs(m_font.get_descender()));

		SetPreferredSize(m_text_outline.width(), m_text_outline.height());
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
		m_status.set(ButtonPressed);
		m_status.set(ButtonDown);

		if(m_status[ButtonCheckable]) {
			m_status[ButtonCheckedOrigin] = m_status[ButtonChecked];
			m_status[ButtonChecked] = !m_status[ButtonChecked];
		}

		Refresh();
		return Accept;
	}

	ResponseType AbstractButton::MouseReleaseEvent(const MouseEvent& event)
	{
		int fire_event = 0;	// 0: no event, 1: click event, 2: toggled event

		if(m_status[ButtonCheckable]) {
			if(m_status[ButtonPressed]) {
				fire_event = 2;
			}
		} else {
			if(m_status[ButtonPressed] && m_status[ButtonDown]) {
				fire_event = 1;
			}
		}

		m_status.reset(ButtonPressed);
		m_status.reset(ButtonDown);

		Refresh();

		switch (fire_event) {

			case 0:
				break;

			case 1:
				DBG_PRINT_MSG("%s", "fire clicked event");
				m_clicked.fire();
				break;

			case 2: {
				if(m_status[ButtonChecked] != m_status[ButtonCheckedOrigin]) {
					DBG_PRINT_MSG("file toggle event: %s", m_status[ButtonChecked] ? "on" : "off");
					m_toggled.fire(m_status[ButtonChecked]);
				}
				break;
			}

			default:
				break;
		}

		return Accept;
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

	size_t AbstractButton::GetValidTextSize()
	{
		size_t width = 0;

		size_t str_len = m_text.length();

		width = m_font.GetTextWidth(m_text, str_len);

		if(width > size().width()) {
			while(str_len > 0) {
				width = m_font.GetTextWidth(m_text, str_len);
				if(width < size().width()) break;
				str_len--;
			}
		}

		return str_len;
	}

} /* namespace BlendInt */

