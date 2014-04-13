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

#include <assert.h>

#include <BlendInt/Gui/AbstractDequeContainer.hpp>

namespace BlendInt {

	DequeIterator AbstractDequeContainer::default_iterator;

	AbstractDequeContainer::AbstractDequeContainer ()
	: AbstractContainerExt()
	{
		m_sub_widgets.reset(new WidgetDequeExt);
	}

	AbstractDequeContainer::~AbstractDequeContainer ()
	{
		for(WidgetDequeExt::iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
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

	bool AbstractDequeContainer::AddSubWidget (AbstractWidgetExt* widget)
	{
		if(!widget) return false;

		if(widget->container()) {

			if(widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s", widget->name().c_str(), widget->container()->name().c_str());
				return true;
			} else {
				// Set widget's container to 0
				AbstractContainerExt::RemoveSubWidget(widget->container(), widget);
			}

		}

		m_sub_widgets->push_back(widget);

		SetContainer(widget, this);

		// TODO: set layer and lock the geometry of the sub widget

		events()->connect(widget->destroyed(), this, &AbstractDequeContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractDequeContainer::RemoveSubWidget (AbstractWidgetExt* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
				&AbstractDequeContainer::OnSubWidgetDestroyed);

		WidgetDequeExt::iterator it = std::find(m_sub_widgets->begin(),
				m_sub_widgets->end(), widget);

		if (it != m_sub_widgets->end()) {
			m_sub_widgets->erase(it);

			SetContainer(widget, 0);

			return true;

		} else {

			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
					widget->name().c_str(), name().c_str());
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

	void AbstractDequeContainer::OnSubWidgetDestroyed (AbstractWidgetExt* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s", widget->name().c_str(), name().c_str());

		RemoveSubWidget(widget);
	}

}
