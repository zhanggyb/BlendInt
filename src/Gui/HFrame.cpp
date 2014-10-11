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

#include <BlendInt/Gui/HFrame.hpp>

namespace BlendInt {

	HFrame::HFrame()
	: Frame (),
	  space_(0)
	{

	}

	HFrame::~HFrame()
	{
	}

	void HFrame::AddWidget(Widget* widget, bool append)
	{
		int x = GetLastPosition();
		int y = margin_.bottom();
		int h = size().height() - margin_.vsum();

		if(PushBackSubWidget(widget)) {

			Size prefer = widget->GetPreferredSize();

			SetSubWidgetPosition(widget, x, y);

			if(widget->IsExpandY()) {

				ResizeSubWidget(widget, prefer.width(), h);

			} else {

				if(widget->size().height() > h) {
					ResizeSubWidget(widget, prefer.width(), h);
				} else {
					ResizeSubWidget(widget, prefer.width(), widget->size().height());
					SetSubWidgetPosition(widget, x,
									y + (h - widget->size().height()) / 2);
				}

			}
		}

	}

	void HFrame::InsertWidget(int index, Widget* widget)
	{
	}

	int HFrame::GetLastPosition() const
	{
		int x = margin_.left();

		if (last_child()) {
			x = last_child()->position().x();
			x += last_child()->size().width() + space_;
		}

		return x;
	}

}
