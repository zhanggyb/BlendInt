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

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <BlendInt/Gui/AbstractButton.hpp>
#include <BlendInt/Gui/ButtonGroup.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Margin AbstractButton::kPadding = Margin(2, 2, 2, 2);

	int AbstractButton::kIconTextSpace = 2;

	AbstractButton::AbstractButton ()
	: Widget(),
	  group_(0)
	{

	}

	AbstractButton::AbstractButton(const String& text)
	: Widget(),
	  group_(0)
	{
		text_.reset(new Text(text));
	}

	AbstractButton::AbstractButton(const RefPtr<AbstractIcon>& icon)
	: Widget(),
	  group_(0)
	{
		icon_ = icon;
	}

	AbstractButton::AbstractButton(const RefPtr<AbstractIcon>& icon,
			const String& text)
	: Widget(),
	  group_(0)
	{
		icon_ = icon;
		text_.reset(new Text(text));
	}

	AbstractButton::~AbstractButton ()
	{

	}

	Size AbstractButton::GetPreferredSize () const
	{
		int w = 0;
		int h = 0;

		if(icon_) {
			w = icon_->size().width();
			h = icon_->size().height();
		}

		w += kIconTextSpace;

		Font font;	// default font
		if(text_) {
			font = text_->font();
			w += text_->size().width();
		}

		h = std::max(h, font.height());

		if (w == kIconTextSpace) {
			w = h;
		}

		w += pixel_size(kPadding.hsum());
		h += pixel_size(kPadding.vsum());

		return Size(w, h);
	}

	void AbstractButton::SetIcon (const RefPtr<AbstractIcon>& icon)
	{
		icon_ = icon;

		RequestRedraw();
	}

	void AbstractButton::SetText (const String& text)
	{
		if(text_) {
			text_->SetText(text);
		} else {
			text_.reset(new Text(text));
		}

		RequestRedraw();
	}

	void AbstractButton::SetFont (const Font& font)
	{
		if(text_) {
			text_->SetFont(font);

			RequestRedraw();
		}
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
	
	void AbstractButton::DrawIconText()
	{
		int w = size().width() - pixel_size(kPadding.hsum());
		int h = size().height() - pixel_size(kPadding.vsum());
		int x = pixel_size(kPadding.left());
		int y = size().height() / 2;

		if(icon_) {
			if(icon_->size().height() <= h) {

				if(icon_->size().width() <= w) {

					icon_->Draw(x + icon_->size().width() / 2, y);
					x += icon_->size().width();
					x += kIconTextSpace;
					w -= icon_->size().width();
					w -= kIconTextSpace;
				}

			}
		}

		if (text_) {

			if(text_->size().height() <= h) {

				y = (size().height() - text_->font().height()) / 2 - text_->font().descender();

				// A workaround for Adobe Source Han Sans
				int diff = text_->font().ascender() - text_->font().descender();
				if(diff < text_->font().height()) {
					y += (text_->font().height() - diff - 1) / 2;
				}

				if(text_->size().width() < w) {
					x += (w - text_->size().width()) / 2;
					text_->Draw(x, y);
				} else {
					text_->DrawWithin(x, y, w);
				}

			}

		}
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

} /* namespace BlendInt */
