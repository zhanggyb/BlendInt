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

#include <BlendInt/Gui/VFrame.hpp>

namespace BlendInt {

	VFrame::VFrame()
	: MultipleFrame(),
	  space_(0)
	{

	}

	VFrame::~VFrame()
	{
	}

	void VFrame::AddWidget(Widget* widget, bool append)
	{
		int x = margin_.left();
		int y = GetLastPosition();
		int w = size().width() - margin_.hsum();

		if(PushBackSubWidget(widget)) {

			Size prefer = widget->GetPreferredSize();

			y = y - prefer.height();

			SetSubWidgetPosition(widget, x, y);

			if(widget->IsExpandX()) {
				ResizeSubWidget(widget, w, prefer.height());
			} else {
				if(widget->size().width() > w) {
					ResizeSubWidget(widget, w, prefer.height());
				} else {
					ResizeSubWidget(widget, widget->size().width(), prefer.height());
				}
			}
		}

	}

	void VFrame::InsertWidget(int index, Widget* widget)
	{
	}

	void VFrame::LayoutWidgets()
	{
	}

	int VFrame::GetLastPosition() const
	{
		int y = size().height() - margin_.top();

		if(last_child()) {
			y = last_child()->position().y();
			y -= space_;
		}

		return y;
	}

}
