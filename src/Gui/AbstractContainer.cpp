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

#include <algorithm>

#include <BlendInt/Gui/AbstractContainer.hpp>

#include <BlendInt/Service/ContextManager.hpp>

namespace BlendInt {

	void AbstractContainerExt::SetMargin (const Margin& margin)
	{
		if (m_margin.equal(margin))
			return;

		if(Update(UpdateRequest(Predefined, ContainerMargin, &margin))) {
			m_margin = margin;
		}
	}

	void AbstractContainerExt::SetMargin (int left, int right, int top, int bottom)
	{
		if (m_margin.equal(left, right, top, bottom))
			return;

		Margin new_margin(left, right, top, bottom);

		if(Update(UpdateRequest(Predefined, ContainerMargin, &new_margin))) {
			m_margin = new_margin;
		}
	}

	AbstractContainer::AbstractContainer ()
	{
		m_sub_widgets.reset(new WidgetDeque);
		ContextManager::instance->AddSubWidget(this);
	}

	AbstractContainer::~AbstractContainer ()
	{
		for(WidgetDeque::iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
		{
			// check if need to delete
			if((*it)->managed()) {

				(*it)->destroyed().disconnectOne(this,
						&AbstractContainer::OnSubWidgetDestroyed);

				// Delete the widget if it's not referenced by any RefPtr
				if((*it)->count() == 0) {
					delete *it;
				}

			} else {

				// Move the sub widget to context manager
				(*it)->destroyed().disconnectOne(this,
				        &AbstractContainer::OnSubWidgetDestroyed);
				(*it)->m_container = 0;

				ContextManager::instance->AddSubWidget(*it);
				(*it)->m_flag.reset(WidgetFlagInContainer);

			}
		}
	}

	void AbstractContainer::SetMargin (const Margin& margin)
	{
		if (m_margin.equal(margin))
			return;

		if(Update(UpdateRequest(Predefined, ContainerMargin, &margin))) {
			m_margin = margin;
		}
	}

	void AbstractContainer::SetMargin (int l, int r, int t, int b)
	{
		if (m_margin.equal(l, r, t, b))
			return;

		Margin new_margin(l, r, t, b);

		if(Update(UpdateRequest(Predefined, ContainerMargin, &new_margin))) {
			m_margin = new_margin;
		}
	}

	bool AbstractContainer::FindSubWidget (AbstractWidget* widget)
	{
		WidgetDeque::iterator it = std::find(m_sub_widgets->begin(), m_sub_widgets->end(), widget);

		if(it != m_sub_widgets->end())
			return true;
		else
			return false;
	}

	bool AbstractContainer::AppendSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;
		if(widget == ContextManager::instance) {
			DBG_PRINT_MSG("%s", "Cannot add context manager in container");
			return false;
		}

		AbstractContainer* old_container = 0;

		// Remove the widget from the original container
		if(widget->container()) {

			if(widget->container() == this) {
				DBG_PRINT_MSG("%s", "The widget is already in this container");
				return true;
			}

			if(widget->container() == ContextManager::instance) {
				ContextManager::instance->Unregister(widget);
			} else {
				old_container = dynamic_cast<AbstractContainer*>(widget->container());
			}

		}

		m_sub_widgets->push_back(widget);

		if(old_container) {
			old_container->RemoveSubWidgetOnly(widget);
		}

		widget->m_container = this;
		widget->m_flag.set(WidgetFlagInContainer);

		// TODO: set layer and lock the geometry of the sub widget

		events()->connect(widget->destroyed(), this, &AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::InsertSubWidget (size_t index, AbstractWidget* widget)
	{
		if(!widget) return false;
		if(widget == ContextManager::instance) {
			DBG_PRINT_MSG("%s", "Cannot add context manager in container");
			return false;
		}
		if(index > (m_sub_widgets->size() - 1)) {
			DBG_PRINT_MSG("Out of range: index %ld is not valid", index);
			return false;
		}

		AbstractContainer* old_container = 0;

		// Remove the widget from the original container
		if(widget->container()) {

			if(widget->container() == this) {
				DBG_PRINT_MSG("%s", "The widget is already in this container");
				return true;
			}

			if(widget->container() == ContextManager::instance) {
				ContextManager::instance->Unregister(widget);
			} else {
				old_container = dynamic_cast<AbstractContainer*>(widget->container());
			}

		}

		WidgetDeque::iterator it = m_sub_widgets->begin();
		std::advance(it, index);
		m_sub_widgets->insert(it, widget);

		if(old_container) {
			old_container->RemoveSubWidgetOnly(widget);
		}

		widget->m_container = this;
		widget->m_flag.set(WidgetFlagInContainer);

		// TODO: set layer and lock the geometry of the sub widget

		events()->connect(widget->destroyed(), this, &AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}


	bool AbstractContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if (widget) {

			if (widget->container() == this) {

				widget->destroyed().disconnectOne(this,
				        &AbstractContainer::OnSubWidgetDestroyed);
				widget->m_container = 0;

				ContextManager::instance->AddSubWidget(widget);

				WidgetDeque::iterator it = std::find(m_sub_widgets->begin(),
				        m_sub_widgets->end(), widget);

				if (it != m_sub_widgets->end()) {
					m_sub_widgets->erase(it);
					widget->m_flag.reset(WidgetFlagInContainer);
				} else {
					DBG_PRINT_MSG(
					        "Warning: object %s is not found in container %s",
					        widget->name().c_str(), name().c_str());
					return false;
				}

			} else {
				return false;
			}

			return true;

		} else {
			return false;
		}
	}

#ifdef DEBUG

	void AbstractContainer::print()
	{
		for(WidgetDeque::iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
		{
			// check if need to delete
			DBG_PRINT_MSG("Sub widget %s in %s", (*it)->name().c_str(), name().c_str());
		}
	}

#endif

	void AbstractContainer::ClearSubWidgets ()
	{
		for(WidgetDeque::iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
		{
			(*it)->destroyed().disconnectOne(this, &AbstractContainer::OnSubWidgetDestroyed);

			if((*it)->managed() && (*it)->count() == 0) {
				delete *it;
			}
		}

		m_sub_widgets->clear();
	}

	void AbstractContainer::OnSubWidgetDestroyed(AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s", widget->name().c_str(), name().c_str());

		WidgetDeque::iterator it = std::find(m_sub_widgets->begin(),
		        m_sub_widgets->end(), widget);
		if (it != m_sub_widgets->end()) {
			m_sub_widgets->erase(it);
		} else {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
			        widget->name().c_str(), name().c_str());
		}

		widget->destroyed().disconnectOne(this, &AbstractContainer::OnSubWidgetDestroyed);
	}

	void AbstractContainer::RemoveSubWidgetOnly (AbstractWidget* widget)
	{
		if(widget) {
			widget->destroyed().disconnectOne(this, &AbstractContainer::OnSubWidgetDestroyed);

			WidgetDeque::iterator it = std::find(m_sub_widgets->begin(),
			        m_sub_widgets->end(), widget);
			if (it != m_sub_widgets->end()) {
				m_sub_widgets->erase(it);
			} else {
				DBG_PRINT_MSG("Warning: object %s is not found in container %s",
				        widget->name().c_str(), name().c_str());
			}

		}
	}

	void AbstractContainer::MoveSubWidgets (int offset_x, int offset_y)
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++) {
			SetPosition(*it, (*it)->position().x() + offset_x,
							(*it)->position().y() + offset_y);
		}
	}

	void AbstractContainer::ResizeSubWidgets(const Size& size)
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
		{
			Resize((*it), size);
		}
	}

	void AbstractContainer::ResizeSubWidgets(unsigned int w, unsigned int h)
	{
		for (WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			Resize((*it), w, h);
		}
	}

}
