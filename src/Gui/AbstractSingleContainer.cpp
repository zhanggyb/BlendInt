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

#include <BlendInt/Gui/AbstractSingleContainer.hpp>

namespace BlendInt {

	AbstractSingleContainer::AbstractSingleContainer ()
	: m_sub_widget(0)
	{
	}

	AbstractSingleContainer::~AbstractSingleContainer ()
	{
		if(m_sub_widget) {

			m_sub_widget->destroyed().disconnectOne(this,
							&AbstractSingleContainer::OnSubWidgetDestroyed);
			AbstractContainer::SetContainer(m_sub_widget, 0);

			if(m_sub_widget->managed() && (m_sub_widget->count() == 0)) {
				delete m_sub_widget;
			}

			m_sub_widget = 0;
		}
	}
	
	bool AbstractSingleContainer::AddSubWidget (AbstractWidget* widget)
	{
		if(!widget)
			return false;

		if(widget->container()) {

			if(widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
									widget->name().c_str(),
									widget->container()->name().c_str());
				return true;
			} else {
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}
		}

		m_sub_widget = widget;
		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractSingleContainer::OnSubWidgetDestroyed);

		return true;
	}
	
	bool AbstractSingleContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
						&AbstractSingleContainer::OnSubWidgetDestroyed);

		m_sub_widget = 0;
		SetContainer(widget, 0);

		return true;
	}
	
	IteratorPtr AbstractSingleContainer::CreateIterator (
					const DeviceEvent& event)
	{
		RefPtr<SingleIterator> ret(new SingleIterator(m_sub_widget));

		return ret;
	}

	void AbstractSingleContainer::OnSubWidgetDestroyed(AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s",
						widget->name().c_str(), name().c_str());

		RemoveSubWidget(widget);
	}

}
