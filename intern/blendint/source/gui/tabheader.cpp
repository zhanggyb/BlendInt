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

#include <gui/tabheader.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	TabHeader::TabHeader()
	: AbstractWidget()
	{
		set_size(320, 20);

		events()->connect(group_.button_index_clicked(), &m_button_index_clicked, &Cpp::Event<int>::fire);
		//events()->connect(m_group.button_index_clicked(), this, &TabHeader::OnButtonIndexClicked);
		events()->connect(group_.button_index_toggled(), this, &TabHeader::OnButtonIndexToggled);

		// FIXME: cannot use the following line
		//events()->connect(m_group.button_index_toggled(), &m_button_index_toggled, &Cpp::Event<int, bool>::fire);
	}

	TabHeader::~TabHeader()
	{
	}

	void TabHeader::AddButton (TabButton* button)
	{
		int x = GetLastPosition ();
		int y = 0;
		int h = size().height();

		if (PushBackSubView(button)) {

			MoveSubViewTo(button, x, y);
			if (button->IsExpandY()) {
				ResizeSubView(button, button->size().width(), h);
			} else {

				if (button->size().height() > h) {
					ResizeSubView(button, button->size().width(), h);
				}

			}

			group_.AddButton(button);

			if(group_.button_count() == 1) {
				button->SetChecked(true);
			}
		}
	}

	bool TabHeader::IsExpandX () const
	{
		return true;
	}

	Size TabHeader::GetPreferredSize () const
	{
		Size prefer(320, 20);

		if(first_subview() == 0) {
			BlendInt::Font font;
			int max_font_height = font.height();
			prefer.set_height(max_font_height);
		} else {
			Size tmp_size;

			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					prefer.add_width(tmp_size.width());
					prefer.set_height(std::max(prefer.height(), tmp_size.height()));
				}
			}

		}

		return prefer;
	}

	Response TabHeader::Draw (AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	void TabHeader::OnButtonIndexToggled(int index, bool toggled)
	{
		m_button_index_toggled.fire(index, toggled);
	}

	int TabHeader::GetLastPosition() const
	{
		int x = 0;

		if(subs_count()) {
			x = last_subview()->position().x()+ last_subview()->size().width();
		}

		return x;
	}

}
