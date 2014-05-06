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

#include <BlendInt/Gui/SingleLayout.hpp>

namespace BlendInt {

	SingleLayout::SingleLayout(AbstractContainer* container)
	: AbstractLayoutExt(container), m_sub_widget(0)
	{

	}

	SingleLayout::SingleLayout(AbstractContainer* container, AbstractWidget* sub_widget)
	: AbstractLayoutExt(container), m_sub_widget(sub_widget)
	{

	}

	SingleLayout::~SingleLayout()
	{

	}

	void SingleLayout::Fill ()
	{
		int x = container()->position().x() + container()->margin().left();
		int y = container()->position().y() + container()->margin().bottom();

		unsigned int w = container()->size().width()
						- container()->margin().left()
						- container()->margin().right();
		unsigned int h = container()->size().height()
						- container()->margin().top()
						- container()->margin().bottom();

		Resize(m_sub_widget, w, h);
		SetPosition(m_sub_widget, x, y);

		if (m_sub_widget->size().width() < w) {
			SetPosition(m_sub_widget,
							x + (w - m_sub_widget->size().width()) / 2, y);
		}

		if (m_sub_widget->size().height() < h) {
			SetPosition(m_sub_widget, x,
							y + (h - m_sub_widget->size().height() / 2));
		}
	}

}
