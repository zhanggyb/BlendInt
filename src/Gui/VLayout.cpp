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

#include <set>

#include <boost/smart_ptr.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	VLayoutExt::VLayoutExt (Context* context, int alignment, int space)
	: AbstractLayoutExt(context),
	  m_alignment(alignment),
	  m_space(space)
	{

	}

	VLayoutExt::~VLayoutExt ()
	{
	}

	bool VLayoutExt::AddLayoutItem(AbstractWidget* widget)
	{
		if(!widget) return false;

		WidgetDeque::iterator it = std::find(m_widgets.begin(), m_widgets.end(), widget);
		if(it == m_widgets.end()) {
			m_widgets.push_back(widget);
			return true;
		}

		return false;
	}

	bool VLayoutExt::RemoveLayoutItem(AbstractWidget* widget)
	{
		if(!widget) return false;

		WidgetDeque::iterator it = std::find(m_widgets.begin(), m_widgets.end(), widget);
		if(it != m_widgets.end()) {
			m_widgets.erase(it);
			return true;
		}

		return false;
	}

	void VLayoutExt::Fill ()
	{
		if(m_widgets.empty()) return;

		Distribute();

		Align();
	}

	void VLayoutExt::Distribute()
	{
		int x = context()->margin().left();
		int y = context()->size().height() - context()->margin().top();

		unsigned int width = context()->size().width() - context()->margin().left() - context()->margin().right();
		unsigned int height = context()->size().height() - context()->margin().top() - context()->margin().bottom();

		Size preferred_size;
		AbstractWidget* widget = 0;

		std::set<AbstractWidget*> y_unexpandable_widgets;

		for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
		{
			widget = *it;
			if(!widget->expand_y()) {
				y_unexpandable_widgets.insert(widget);
			}
		}

		if(y_unexpandable_widgets.empty()) {	// all expandable along y

			unsigned int average_height = height / m_widgets.size();
			for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
			{
				widget = *it;
				y -= average_height;
				widget->SetPosition(widget->position().x(), y);
				widget->Resize(widget->size().width(), average_height);
				y -= m_space;	// space
			}

		} else {

			unsigned int total_fixed_height = 0;

			for(std::set<AbstractWidget*>::iterator it = y_unexpandable_widgets.begin(); it != y_unexpandable_widgets.end(); it++)
			{
				widget = *it;
				preferred_size = widget->GetPreferredSize();
				total_fixed_height += preferred_size.height();
			}

			if(total_fixed_height < height) {

				unsigned int tmp;

				unsigned int averate_expand_height = 0;
				if(y_unexpandable_widgets.size() < m_widgets.size()) {
					averate_expand_height = (height - total_fixed_height) / (m_widgets.size() - y_unexpandable_widgets.size());
				}

				for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
				{
					widget = *it;
					if(!widget->expand_y()) {
						tmp = widget->GetPreferredSize().height();
						y -= tmp;
						widget->SetPosition(widget->position().x(), y);
						widget->Resize(widget->size().width(), tmp);
						y -= m_space; // space
					} else {
						y -= averate_expand_height;
						widget->SetPosition(widget->position().x(), y);
						widget->Resize(widget->size().width(), averate_expand_height);
						y -= m_space;	// space
					}
				}

			}

		}
	}

	void VLayoutExt::Align ()
	{
		int x = context()->margin().left();
		int y = context()->size().height() - context()->margin().top();

		unsigned int width = context()->size().width() - context()->margin().left() - context()->margin().right();
		unsigned int height = context()->size().height() - context()->margin().top() - context()->margin().bottom();

		AbstractWidget* widget = 0;
		for(WidgetDeque::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
		{
			widget = *it;

			if (widget->expand_x()) {
				widget->Resize(width, widget->size().height());
				widget->SetPosition(x, widget->position().y());
			} else {

				if (m_alignment & AlignLeft) {
					widget->SetPosition(x, widget->position().y());
				} else if (m_alignment & AlignRight) {
					widget->SetPosition(x + (width - widget->size().width()), widget->position().y());
				} else if (m_alignment & AlignVerticalCenter) {
					widget->SetPosition(x + (width - widget->size().width()) / 2, widget->position().y());
				}

			}
		}
	}

}
