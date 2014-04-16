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

	//DequeIterator AbstractDequeContainer::default_iterator;

	AbstractDequeContainer::AbstractDequeContainer ()
	: AbstractContainer()
	{
		m_sub_widgets.reset(new WidgetDeque);
	}

	AbstractDequeContainer::~AbstractDequeContainer ()
	{
		ClearSubWidgets();
	}

	bool AbstractDequeContainer::AddSubWidget (AbstractWidget* widget)
	{
		if (!widget)
			return false;

		if (widget->container()) {

			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name().c_str(),
								widget->container()->name().c_str());
				return true;
			} else {
				// Set widget's container to 0
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}

		}

		m_sub_widgets->push_back(widget);

		SetContainer(widget, this);

		// TODO: set layer and lock the geometry of the sub widget

		events()->connect(widget->destroyed(), this,
						&AbstractDequeContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractDequeContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
						&AbstractDequeContainer::OnSubWidgetDestroyed);

		WidgetDeque::iterator it = std::find(m_sub_widgets->begin(),
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

	bool AbstractDequeContainer::FindSubWidget (AbstractWidget* widget)
	{
		WidgetDeque::iterator it = std::find(m_sub_widgets->begin(), m_sub_widgets->end(), widget);

		if(it != m_sub_widgets->end())
			return true;
		else
			return false;
	}
	
	bool AbstractDequeContainer::AppendSubWidget (AbstractWidget* widget)
	{
		return AddSubWidget(widget);
	}
	
	bool AbstractDequeContainer::InsertSubWidget (size_t index,
					AbstractWidget* widget)
	{
		if(!widget) return false;

		if(index > (m_sub_widgets->size() - 1)) {
			DBG_PRINT_MSG("Out of range: index %ld is not valid", index);
			return false;
		}

		if (widget->container()) {

			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name().c_str(),
								widget->container()->name().c_str());
				return true;
			} else {
				// Set widget's container to 0
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}

		}

		WidgetDeque::iterator it = m_sub_widgets->begin();
		std::advance(it, index);
		m_sub_widgets->insert(it, widget);

		SetContainer(widget, this);

		// TODO: set layer and lock the geometry of the sub widget

		events()->connect(widget->destroyed(), this,
						&AbstractDequeContainer::OnSubWidgetDestroyed);

		return true;
	}
	
	void AbstractDequeContainer::MoveSubWidgets (int offset_x, int offset_y)
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++) {
			SetPosition(*it, (*it)->position().x() + offset_x,
							(*it)->position().y() + offset_y);
		}
	}
	
	void AbstractDequeContainer::ResizeSubWidgets (const Size& size)
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++) {
			Resize((*it), size);
		}
	}
	
	void AbstractDequeContainer::ResizeSubWidgets (unsigned int w,
					unsigned int h)
	{
		for (WidgetDeque::iterator it = sub_widgets()->begin();
						it != sub_widgets()->end(); it++) {
			Resize((*it), w, h);
		}
	}
	
	void AbstractDequeContainer::ClearSubWidgets ()
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++)
		{
			(*it)->destroyed().disconnectOne(this,
							&AbstractDequeContainer::OnSubWidgetDestroyed);
			AbstractContainer::SetContainer(*it, 0);

			if ((*it)->managed() && ((*it)->count() == 0)) {
				delete *it;
			}
		}

		m_sub_widgets->clear();
	}

	IteratorPtr AbstractDequeContainer::CreateIterator (const DeviceEvent& event)
	{
		RefPtr<DequeIterator> ret(new DequeIterator(m_sub_widgets.get()));

		return ret;
	}

	void AbstractDequeContainer::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s",
						widget->name().c_str(), name().c_str());

		RemoveSubWidget(widget);
	}

}
