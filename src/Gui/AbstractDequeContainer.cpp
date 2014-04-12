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

#include <BlendInt/Gui/AbstractDequeContainer.hpp>

namespace BlendInt {

	DequeIterator AbstractDequeContainer::default_iterator;

	AbstractDequeContainer::AbstractDequeContainer ()
	{
		m_sub_widgets.reset(new WidgetDeque);
	}

	AbstractDequeContainer::~AbstractDequeContainer ()
	{
		for(WidgetDeque::iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
		{
			// check if need to delete
			if((*it)->managed()) {

				(*it)->destroyed().disconnectOne(this,
						&AbstractDequeContainer::OnSubWidgetDestroyed);

				// Delete the widget if it's not referenced by any RefPtr
				if((*it)->count() == 0) {
					delete *it;
				}

			} else {

				// Move the sub widget to context manager
				(*it)->destroyed().disconnectOne(this,
				        &AbstractDequeContainer::OnSubWidgetDestroyed);
				//(*it)->m_container = 0;

				//ContextManager::instance->AddSubWidget(*it);
				//(*it)->m_flag.reset(WidgetFlagInContainer);

			}
		}
	}

	bool AbstractDequeContainer::AddSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;

		return true;
	}

	bool AbstractDequeContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if (widget) {

			return true;

		} else {
			return false;
		}
	}

	AbstractWidgetIterator* AbstractDequeContainer::First (
        const DeviceEvent& event)
	{
		default_iterator.set_it(m_sub_widgets->begin());
		return &default_iterator;
	}

	bool AbstractDequeContainer::End (const DeviceEvent& event,
        AbstractWidgetIterator* iter)
	{
		DequeIterator* it = dynamic_cast<DequeIterator*>(iter);
		//if(it && (*it) == m_sub_widgets->end())
		if(it){
			return true;
		} else {
			return false;
		}
	}

	void AbstractDequeContainer::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
	}

}
