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

#include <BlendInt/Gui/LinearLayout.hpp>

namespace BlendInt {

	LinearLayout::LinearLayout (AbstractContainer* container)
	: LayoutBase(container),
	  m_orientation(Horizontal),
	  m_alignment(AlignCenter),
	  m_space(2)
	{
	}

	LinearLayout::LinearLayout (AbstractContainer* container,
					Orientation orientation, int alignment, int space)
	: LayoutBase(container),
	  m_orientation(orientation),
	  m_alignment(alignment),
	  m_space(space)
	{
	}

	LinearLayout::~LinearLayout ()
	{
	}

	void LinearLayout::Fill (const WidgetDeque* sub_widgets)
	{
		Point pos;

		pos.set_x(container()->position().x() + container()->margin().left());
		pos.set_y(container()->position().y() + container()->margin().bottom());

		if(m_orientation == Horizontal) {
			DistributeHorizontally(pos, sub_widgets);
			AlignHorizontally(pos, sub_widgets);
		} else {
			DistributeVertically(pos, sub_widgets);
			AlignVertically(pos, sub_widgets);
		}
	}
	
	void LinearLayout::Fill (const Point& start, const WidgetDeque* sub_widgets)
	{
		if(m_orientation == Horizontal) {
			DistributeHorizontally(start, sub_widgets);
			AlignHorizontally(start, sub_widgets);
		} else {
			DistributeVertically(start, sub_widgets);
			AlignVertically(start, sub_widgets);
		}
	}

	void LinearLayout::DistributeHorizontally (const Point& start, const WidgetDeque* sub_widgets)
	{
		int x = start.x();

		unsigned int width = container()->size().width()
						- container()->margin().left()
						- container()->margin().right();

		if (sub_widgets->size()) {
			int average_width = (width - (sub_widgets->size() * m_space))
							/ sub_widgets->size();

			if (average_width > 0) {

				for (WidgetDeque::const_iterator it = sub_widgets->begin();
								it != sub_widgets->end(); it++) {
					Resize(*it, average_width, (*it)->size().height());
					SetPosition(*it, x, (*it)->position().y());
					x += (*it)->size().width() + m_space;
				}

			} else {

				// TODO: set invisiable

			}

		}


	}
	
	void LinearLayout::DistributeVertically (const Point& start,
					const WidgetDeque* sub_widgets)
	{
	}

	void LinearLayout::AlignHorizontally (const Point& start, const WidgetDeque* sub_widgets)
	{
		int y = start.y();

		unsigned int height = container()->size().height() - container()->margin().top() - container()->margin().bottom();

		AbstractWidget* widget = 0;

		for(WidgetDeque::const_iterator it = sub_widgets->begin(); it != sub_widgets->end(); it++)
		{
			widget = *it;

			if(widget->expand_y()) {
				Resize(widget, widget->size().width(), height);
				SetPosition(widget, widget->position().x(), y);
			} else {

				if (m_alignment & AlignTop) {
					SetPosition(widget, widget->position().x(),
					        y + (height - widget->size().height()));
				} else if (m_alignment & AlignBottom) {
					SetPosition(widget, widget->position().x(), y);
				} else if (m_alignment & AlignHorizontalCenter) {
					SetPosition(widget, widget->position().x(),
					        y + (height - widget->size().height()) / 2);
				}

			}
		}
	}
	
	void LinearLayout::AlignVertically (const Point& start,
					const WidgetDeque* sub_widgets)
	{
	}

}
