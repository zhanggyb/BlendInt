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

	AbstractButton::AbstractButton ()
	: Widget(),
	  m_text_length(0)
	{
	}

	AbstractButton::~AbstractButton ()
	{

	}

	Size AbstractButton::GetPreferredSize () const
	{
		Size preferred_size;

		int radius_plus = 0;

		if((round_corner_type() & RoundTopLeft) || (round_corner_type() & RoundBottomLeft)) {
			radius_plus += round_corner_radius();
		}

		if((round_corner_type() & RoundTopRight) || (round_corner_type() & RoundBottomRight)) {
			radius_plus += round_corner_radius();
		}

		int max_font_height = m_font.GetHeight();

		preferred_size.set_height(
		        max_font_height + DefaultButtonPadding().vsum());// top padding: 2, bottom padding: 2

		if (text().empty()) {
			preferred_size.set_width(
			        max_font_height + DefaultButtonPadding().hsum()
			                + radius_plus);
		} else {
			int width = font().GetTextWidth(text());
			preferred_size.set_width(width + DefaultButtonPadding().hsum() + radius_plus);	// left padding: 2, right padding: 2
		}

		return preferred_size;
	}

	void AbstractButton::SetText (const String& text)
	{
		m_text = text;
		m_text_length = UpdateTextPosition(size(), round_corner_type(), round_corner_radius(), text, m_font);
	}

	void AbstractButton::SetFont (const Font& font)
	{
		m_font = font;
		m_text_length = UpdateTextPosition(size(), round_corner_type(), round_corner_radius(), m_text, m_font);
	}

	void AbstractButton::UpdateTextPosition(const Size& size, int round_type, float radius, const String& text)
	{
		m_text_length = UpdateTextPosition(size, round_type, radius, text, m_font);
	}

	size_t AbstractButton::UpdateTextPosition (const Size& size, int round_type, float radius, const String& text, Font& font)
	{
		size_t str_len = 0;

		// If size changed, we need to update the text length for printing too.
		bool cal_width = true;

		int radius_plus = 0;

		if((round_type & RoundTopLeft) || (round_type & RoundBottomLeft)) {
			radius_plus += radius;
		}

		if((round_type & RoundTopRight) || (round_type & RoundBottomRight)) {
			radius_plus += radius;
		}

		int width = size.width() - DefaultButtonPadding().hsum() - radius_plus;
		int height = size.height() - DefaultButtonPadding().vsum();

		if(width <= 0 || height <= 0) {
			return 0;
		}

		if(text.length() == 0) {
			return 0;
		}

		Rect text_outline = font.GetTextOutline(text);

		if(height < text_outline.height()) {
			str_len = 0;
			cal_width = false;
		}

		if(cal_width) {
			if(width < text_outline.width()) {
				str_len = GetValidTextSize(size, text, font);
			} else {
				str_len = text.length();
			}
		}

		font.set_pen((size.width() - text_outline.width()) / 2,
						(size.height() - font.GetHeight()) / 2
										+ std::abs(font.GetDescender()));

		return str_len;
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

	size_t AbstractButton::GetValidTextSize(const Size& size, const String& text, const Font& font)
	{
		int width = 0;
		int str_len = text.length();

		width = font.GetTextWidth(text, str_len, 0);

		int text_width_space = size.width() - DefaultButtonPadding().left() - DefaultButtonPadding().right();

		if(width > text_width_space) {
			while(str_len > 0) {
				width = font.GetTextWidth(text, str_len, 0);
				if(width < text_width_space) break;
				str_len--;
			}
		}

		return str_len;
	}

} /* namespace BlendInt */
