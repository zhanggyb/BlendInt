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

#include <opengl/opengl.hpp>

#include <gui/abstract-button.hpp>
#include <gui/button-group.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	Margin AbstractButton::kPadding = Margin(2, 2, 2, 2);

	int AbstractButton::kIconTextSpace = 2;

	AbstractButton::AbstractButton ()
	: AbstractRoundWidget(),
	  group_(0)
	{

	}

	AbstractButton::AbstractButton(const String& text)
	: AbstractRoundWidget(),
	  group_(0)
	{
		text_.reset(new Text(text));
	}

	AbstractButton::AbstractButton(const RefPtr<AbstractIcon>& icon)
	: AbstractRoundWidget(),
	  group_(0)
	{
		icon_ = icon;
	}

	AbstractButton::AbstractButton(const RefPtr<AbstractIcon>& icon,
			const String& text)
	: AbstractRoundWidget(),
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

	void AbstractButton::PerformHoverIn(AbstractWindow* context)
	{
		m_status[ButtonHover] = 1;

		if(m_status[ButtonPressed]) {
			m_status[ButtonDown] = 1;

			if(m_status[ButtonCheckable]) {
				m_status[ButtonChecked] = !m_status[ButtonChecked];
			}
		}

		RequestRedraw();
	}

	void AbstractButton::PerformHoverOut(AbstractWindow* context)
	{
		m_status[ButtonHover] = 0;

		if(m_status[ButtonPressed]) {
			m_status[ButtonDown] = 0;

			if(m_status[ButtonCheckable]) {
				m_status[ButtonChecked] = !m_status[ButtonChecked];
			}

		}

		RequestRedraw();
	}

	Response AbstractButton::PerformMousePress (AbstractWindow* context)
	{
		if (context->GetMouseButton() == MouseButtonLeft) {
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

	Response AbstractButton::PerformMouseRelease(AbstractWindow* context)
	{
		if (context->GetMouseButton() == MouseButtonLeft) {
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

			return Finish;
		}

		return Ignore;
	}

	Response AbstractButton::PerformMouseMove (AbstractWindow* context)
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
		Rect rect(pixel_size(kPadding.left()),
				pixel_size(kPadding.bottom()),
				size().width() - pixel_size(kPadding.hsum()),
				size().height() - pixel_size(kPadding.vsum()));

		if(icon_) {
			if(icon_->size().height() <= rect.height()) {
				if(icon_->size().width() <= rect.width()) {
					icon_->DrawInRect(rect, AlignLeft | AlignVerticalCenter);
					rect.cut_left(icon_->size().width() + kIconTextSpace);
				}
			}
		}

		if (text_) {
			if(text_->size().height() <= rect.height()) {
				text_->DrawInRect(rect, AlignHorizontalCenter | AlignJustify | AlignBaseline);
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
