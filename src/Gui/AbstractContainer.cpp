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

	AbstractContainer::AbstractContainer ()
	{
		ContextManager::Instance()->Register(this);
	}

	AbstractContainer::~AbstractContainer ()
	{
		if(hover()) {
			ContextManager::Instance()->RemoveWidgetFromHoverDeque(this);
		}

		if(focused()) {
			ContextManager::Instance()->SetFocusedWidget(0);
		}

		for(WidgetDeque::iterator it = m_sub_widgets.begin(); it != m_sub_widgets.end(); it++)
		{
			// check if need to delete
			if((*it)->m_flag[AbstractWidget::WidgetFlagManaged]) {
				delete *it;
			}
		}
	}

	void AbstractContainer::SetMargin (const Margin& margin)
	{
		if (m_margin.equal(margin))
			return;

		if(Update(FrameMargin, &margin)) {
			m_margin = margin;
		}
	}

	void AbstractContainer::SetMargin (int l, int r, int t, int b)
	{
		if (m_margin.equal(l, r, t, b))
			return;

		Margin new_margin(l, r, t, b);

		if(Update(FrameMargin, &new_margin)) {
			m_margin = new_margin;
		}
	}

	void AbstractContainer::AddSubWidget (AbstractWidget* widget)
	{
		if(!widget) return;
		if(widget == ContextManager::Instance()) return;	// Cannot add the context manager

		AbstractContainer* old_container = 0;

		// Remove the widget from the original container
		if(widget->container()) {

			if(widget->container() == this) return;	// already contained in this

			if(widget->container() == ContextManager::Instance()) {

				ContextManager::Instance()->Unregister(widget);

			} else {
				old_container = dynamic_cast<AbstractContainer*>(widget->container());
			}

		}

		m_sub_widgets.push_back(widget);

		if(old_container) {
			old_container->RemoveSubWidgetOnly(widget);
		} else {
			DBG_PRINT_MSG("%s", "The widget's old container is not identified");
		}

		widget->m_container = this;
		widget->m_flag.set(WidgetFlagInContainer);

		events()->connect(widget->destroyed(), this, &AbstractContainer::OnSubWidgetDestroyed);
	}

	void AbstractContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if(widget) {
			//widget->SetContainer(0);
			//m_sub_widgets.erase(widget);

			if(widget->container() == this) {

				widget->destroyed().disconnectOne(this, &AbstractContainer::OnSubWidgetDestroyed);

				widget->m_container = 0;

				ContextManager::Instance()->Register(widget);

				WidgetDeque::iterator it = std::find(m_sub_widgets.begin(), m_sub_widgets.end(), widget);
				if(it != m_sub_widgets.end()) {
					m_sub_widgets.erase(it);

					widget->m_flag.reset(WidgetFlagInContainer);

				} else {
					DBG_PRINT_MSG("Warning: object %s is not found in container %s", widget->name().c_str(), name().c_str());
				}

			}

		}
	}

	void AbstractContainer::OnSubWidgetDestroyed(AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s", widget->name().c_str(), name().c_str());

		WidgetDeque::iterator it = std::find(m_sub_widgets.begin(),
		        m_sub_widgets.end(), widget);
		if (it != m_sub_widgets.end()) {
			m_sub_widgets.erase(it);
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

			WidgetDeque::iterator it = std::find(m_sub_widgets.begin(),
			        m_sub_widgets.end(), widget);
			if (it != m_sub_widgets.end()) {
				m_sub_widgets.erase(it);
			} else {
				DBG_PRINT_MSG("Warning: object %s is not found in container %s",
				        widget->name().c_str(), name().c_str());
			}

		}
	}


}
