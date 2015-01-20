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
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Gui/ButtonGroup.hpp>

namespace BlendInt {

	Margin AbstractButton::kDefaultPadding = Margin(2, 2, 2, 2);

	int AbstractButton::icon_text_space = 2;

	AbstractButton::AbstractButton ()
	: Widget(),
	  text_length_(0),
	  group_(0)
	{
	}

	AbstractButton::~AbstractButton ()
	{

	}

	Size AbstractButton::GetPreferredSize () const
	{
		Size preferred_size;

		float radius_plus = 0;

		if ((round_type() & RoundTopLeft) || (round_type() & RoundBottomLeft)) {
			radius_plus += round_radius();
		}

		if ((round_type() & RoundTopRight)
				|| (round_type() & RoundBottomRight)) {
			radius_plus += round_radius();
		}

		int max_font_height = font_.height();

		preferred_size.set_height(
				max_font_height + kDefaultPadding.vsum() * Context::theme->pixel()); // top padding: 2, bottom padding: 2

		if (text_.empty()) {
			preferred_size.set_width(
					max_font_height + kDefaultPadding.hsum()
							+ radius_plus);
		} else {
			int width = font().GetTextWidth(text());
			preferred_size.set_width(
					width + kDefaultPadding.hsum() + radius_plus); // left padding: 2, right padding: 2
		}

		return preferred_size;
	}

	void AbstractButton::SetText (const String& text)
	{
		text_ = text;
		text_length_ = UpdateTextPosition(size(), round_type(), round_radius(), text, font_);
	}

	void AbstractButton::SetFont (const Font& font)
	{
		font_ = font;
		text_length_ = UpdateTextPosition(size(), round_type(), round_radius(), text_, font_);
	}

	void AbstractButton::UpdateTextPosition(const Size& size, int round_type, float radius, const String& text)
	{
		text_length_ = UpdateTextPosition(size, round_type, radius, text, font_);
	}

	int AbstractButton::UpdateTextPosition (const Size& size, int round_type, float radius, const String& text, Font& font)
	{
		if(text.length() == 0) {
			return 0;
		}

		int text_length = 0;

		// If size changed, we need to update the text length for printing too.
		bool cal_width = true;

		float radius_plus = 0.f;
		int x = kDefaultPadding.left(); int y = kDefaultPadding.bottom();

		if((round_type & RoundTopLeft) || (round_type & RoundBottomLeft)) {
			radius_plus += radius;
			x += (int)radius;
		}

		if((round_type & RoundTopRight) || (round_type & RoundBottomRight)) {
			radius_plus += radius;
		}

		int valid_width = size.width() - kDefaultPadding.hsum() - (int)radius_plus;
		int valid_height = size.height() - kDefaultPadding.vsum();

		if(valid_width <= 0 || valid_height <= 0) {
			return 0;
		}

		Rect text_outline;	// = font.GetTextOutline(text);

		if(valid_height < text_outline.height()) {
			text_length = 0;
			cal_width = false;
		}

		if(cal_width) {
			if(valid_width < text_outline.width()) {
				text_length = GetValidTextLength(text, font, valid_width);
			} else {
				text_length = text.length();
				x = (size.width() - text_outline.width()) / 2;
			}
			y = (size.height() - font.height()) / 2 + std::abs(font.descender());
		}

		// font.set_pen(x, y);

		return text_length;
	}

	void AbstractButton::PerformHoverIn(const Context* context)
	{
		if(m_status[ButtonPressed]) {
			m_status[ButtonDown] = 1;

			if(m_status[ButtonCheckable]) {
				m_status[ButtonChecked] = !m_status[ButtonChecked];
			}
		}

		RequestRedraw();
	}

	void AbstractButton::PerformHoverOut(const Context* context)
	{
		if(m_status[ButtonPressed]) {
			m_status[ButtonDown] = 0;

			if(m_status[ButtonCheckable]) {
				m_status[ButtonChecked] = !m_status[ButtonChecked];
			}

		}

		RequestRedraw();
	}

	ResponseType AbstractButton::PerformMousePress (const Context* context)
	{
		set_pressed(true);

		if (context->mouse_button() == MouseButtonLeft) {
			m_status.set(ButtonPressed);
			m_status.set(ButtonDown);

			if (m_status[ButtonCheckable]) {
				m_status[ButtonLastChecked] = m_status[ButtonChecked];
				m_status[ButtonChecked] = !m_status[ButtonChecked];
			}

			RequestRedraw();

			pressed_.fire(this);
		}

		return Finish;
	}

	ResponseType AbstractButton::PerformMouseRelease(const Context* context)
	{
		if (context->mouse_button() == MouseButtonLeft) {
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

			RequestRedraw();

			switch (fire_event) {

				case 0:
					break;

				case 1: {
					if(group_) {
						group_->Click(this);
					} else {
						clicked_.fire(this);
					}
					break;
				}

				case 2: {
					if (m_status[ButtonChecked]
									!= m_status[ButtonLastChecked]) {

						if(group_) {
							group_->Toggle(this, m_status[ButtonChecked]);
						} else {
							toggled_.fire(this, m_status[ButtonChecked]);
						}
					}
					break;
				}

				default:
					break;
			}

			m_status.reset(ButtonPressed);
			m_status.reset(ButtonDown);

			released_.fire(this);

			set_pressed(false);

			return Finish;
		}

		set_pressed(false);

		return Ignore;
	}

	ResponseType AbstractButton::PerformMouseMove (const Context* context)
	{
		/*
		if (m_status[ButtonDown]) {
			event->accept(this);
			return;
		}
		*/
		return Finish;
	}
	
	void AbstractButton::SetDown (bool down)
	{
		if(m_status[ButtonCheckable]) {
			if(m_status[ButtonChecked] != down)
				RequestRedraw();

			m_status[ButtonChecked] = down ? 1 : 0;

			if(group_) {
				group_->Toggle(this, m_status[ButtonChecked]);
			} else {
				toggled_.fire(this, m_status[ButtonChecked]);
			}

		} else {

			if(m_status[ButtonDown] != down)
				RequestRedraw();

			m_status[ButtonDown] = down ? 1 : 0;
			if(group_) {
				group_->Click(this);
			} else {
				clicked_.fire(this);
			}
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
			RequestRedraw();

			if(group_) {
				group_->Toggle(this, m_status[ButtonChecked]);
			} else {
				toggled_.fire(this, m_status[ButtonChecked]);
			}
		}
	}

	int AbstractButton::GetValidTextLength(const String& text, const Font& font, int max_width)
	{
		int width = 0;
		int str_len = text.length();

		width = font.GetTextWidth(text, str_len, 0);

		if(width > max_width) {
			while(str_len > 0) {
				width = font.GetTextWidth(text, str_len, 0);
				if(width < max_width) break;
				str_len--;
			}
		}

		return str_len;
	}

} /* namespace BlendInt */
