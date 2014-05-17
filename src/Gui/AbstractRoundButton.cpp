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

#include <BlendInt/Gui/AbstractRoundButton.hpp>

namespace BlendInt {

	AbstractRoundButton::AbstractRoundButton ()
	: AbstractButton(),
		m_text_length(0)
	{
	}

	AbstractRoundButton::~AbstractRoundButton ()
	{
	}

	void AbstractRoundButton::SetRoundType (int type)
	{
		if(round_type() == type) return;
		bool broadcast = false;

		UpdateRequest request(Predefined, FormRoundType, &type);

		if(UpdateTest(request)) {
			Update(request);
			set_round_type(type);
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractRoundButton::SetRadius (float rad)
	{
		if(radius() == rad) return;
		bool broadcast = false;

		UpdateRequest request(Predefined, FormRoundRadius, &rad);

		if(UpdateTest(request)) {
			Update(request);
			set_radius(rad);
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	Size BlendInt::AbstractRoundButton::GetPreferredSize () const
	{
		Size preferred_size;

		int radius_plus = 0;

		if((round_type() & RoundTopLeft) || (round_type() & RoundBottomLeft)) {
			radius_plus += radius();
		}

		if((round_type() & RoundTopRight) || (round_type() & RoundBottomRight)) {
			radius_plus += radius();
		}

		int max_font_height = font().GetHeight();

		preferred_size.set_height(max_font_height + DefaultButtonPadding().top() + DefaultButtonPadding().bottom());	// top padding: 2, bottom padding: 2

		if (text().empty()) {
			preferred_size.set_width(
							max_font_height + DefaultButtonPadding().left()
											+ DefaultButtonPadding().right()
											+ radius_plus);
		} else {
			size_t width = font().GetTextWidth(text());
			preferred_size.set_width(
							static_cast<unsigned int>(width)
											+ DefaultButtonPadding().left()
											+ DefaultButtonPadding().right()
											+ radius_plus);	// left padding: 2, right padding: 2
		}

		return preferred_size;
	}

	void AbstractRoundButton::SetText (const String& text)
	{
		if(text.empty()) {
			return;
		}
		m_text = text;
		UpdateTextPosition(size(), round_type(), radius(), text, m_font);
	}

	void AbstractRoundButton::SetFont (const Font& font)
	{
		m_font = font;

		UpdateTextPosition(size(), round_type(), radius(), m_text, font);
	}
	
	void AbstractRoundButton::UpdateTextPosition (const Size& size, int round_type, float radius, const String& text, const Font& font)
	{
		// If size changed, we need to update the text length for printing too.
		bool cal_width = true;

		int radius_plus = 0;

		if((round_type & RoundTopLeft) || (round_type & RoundBottomLeft)) {
			radius_plus += radius;
		}

		if((round_type & RoundTopRight) || (round_type & RoundBottomRight)) {
			radius_plus += radius;
		}

		int width = size.width() - DefaultButtonPadding().left() - DefaultButtonPadding().right() - radius_plus;
		int height = size.height() - DefaultButtonPadding().top() - DefaultButtonPadding().bottom();

		if(width <= 0 || height <= 0) {
			m_text_length = 0;
			return;
		}

		Rect text_outline = font.GetTextOutline(text);

		if(static_cast<unsigned int>(height) < text_outline.height()) {
			set_text_length(0);
			cal_width = false;
		}

		if(cal_width) {
			if(static_cast<unsigned int>(width) < text_outline.width()) {
				set_text_length(GetValidTextSize());
			}
		}

		set_pen((size.width() - text_outline.width()) / 2,
						(size.height() - font.GetHeight()) / 2
										+ std::abs(font.GetDescender()));
	}

	size_t AbstractRoundButton::GetValidTextSize()
	{
		size_t width = 0;
		size_t str_len = m_text.length();

		width = m_font.GetTextWidth(m_text, str_len, 0);

		unsigned int text_width_space = size().width() - DefaultButtonPadding().left() - DefaultButtonPadding().right();

		if(width > text_width_space) {
			while(str_len > 0) {
				width = m_font.GetTextWidth(m_text, str_len, 0);
				if(width < text_width_space) break;
				str_len--;
			}
		}

		return str_len;
	}

}
