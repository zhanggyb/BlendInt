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
		if(m_orientation == Horizontal) {
			DistributeHorizontally(sub_widgets);
			AlignHorizontally(sub_widgets);
		} else {
			DistributeVertically(sub_widgets);
			AlignVertically(sub_widgets);
		}
	}
	
	void LinearLayout::DistributeHorizontally (const WidgetDeque* sub_widgets)
	{
		int x = container()->position().x() + container()->margin().left();

		unsigned int width = container()->size().width()
						- container()->margin().left()
						- container()->margin().right();
		AbstractWidget* widget = 0;

		if (sub_widgets->size()) {
			int average_width = (width - ((sub_widgets->size() - 1)* m_space))
							/ sub_widgets->size();

			if (average_width > 0) {

				for (WidgetDeque::const_iterator it = sub_widgets->begin();
								it != sub_widgets->end(); it++) {
					widget = *it;
					Resize(widget, average_width, widget->size().height());
					SetPosition(widget, x, widget->position().y());
					x += average_width + m_space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}
	
	void LinearLayout::DistributeVertically (const WidgetDeque* sub_widgets)
	{
		int y = container()->position().y() + container()->size().height() - container()->margin().top();

		unsigned int height = container()->size().height()
						- container()->margin().top()
						- container()->margin().bottom();
		AbstractWidget* widget = 0;

		if (sub_widgets->size()) {
			int average_height = (height - ((sub_widgets->size() - 1)* m_space))
							/ sub_widgets->size();

			if (average_height > 0) {

				for (WidgetDeque::const_iterator it = sub_widgets->begin();
								it != sub_widgets->end(); it++) {
					widget = *it;
					Resize(widget, widget->size().width(), average_height);
					y -= average_height;
					SetPosition(widget, widget->position().x(), y);
					y -= m_space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}

	void LinearLayout::AlignHorizontally (const WidgetDeque* sub_widgets)
	{
		int y = container()->position().y() + container()->margin().bottom();

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
	
	void LinearLayout::AlignVertically (const WidgetDeque* sub_widgets)
	{
		int x = container()->position().x() + container()->margin().left();

		unsigned int w = container()->size().width() - container()->margin().left() - container()->margin().right();

		AbstractWidget* widget = 0;
		for(WidgetDeque::const_iterator it = sub_widgets->begin(); it != sub_widgets->end(); it++)
		{
			widget = *it;

			if (widget->expand_x() ||
					(widget->size().width() > w)) {
				Resize(widget, w, widget->size().height());
				SetPosition(widget, x, widget->position().y());
			} else {

				if (m_alignment & AlignLeft) {
					SetPosition(widget, x, widget->position().y());
				} else if (m_alignment & AlignRight) {
					SetPosition(widget, x + (w - widget->size().width()), widget->position().y());
				} else if (m_alignment & AlignVerticalCenter) {
					SetPosition(widget, x + (w - widget->size().width()) / 2, widget->position().y());
				}

			}
		}

	}

}
