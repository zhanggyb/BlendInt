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

namespace BlendInt {

	AbstractContainer::AbstractContainer ()
	{
	}

	AbstractContainer::~AbstractContainer ()
	{
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

		// Remove the widget from the original container
		if(widget->container()) {

			if(widget->container() == this) return;	// already contained in this

			if(widget->container() == ContextManager::Instance()) {

				ContextManager::Instance()->Unregister(widget);

			} else {

				AbstractContainer* origin = dynamic_cast<AbstractContainer*>(widget->container());

				if(origin) {
					origin->RemoveSubWidget(widget);
				} else {
					DBG_PRINT_MSG("%s", "The widget's container is not identified");
				}

			}

		}

		RefPtr<AbstractWidget> child(widget);
		m_sub_widgets.push_back(child);

		widget->m_container = this;
		widget->m_flag.set(WidgetFlagInContainer);

	}

	void AbstractContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if(widget) {
			//widget->SetContainer(0);
			//m_sub_widgets.erase(widget);

			if(widget->container() == this) {

				RefPtr<AbstractWidget> child(widget);
				WidgetDeque::iterator it = std::find(m_sub_widgets.begin(), m_sub_widgets.end(), child);
				if(it != m_sub_widgets.end()) {
					m_sub_widgets.erase(it);

					widget->m_container = 0;
					widget->m_flag.reset(WidgetFlagInContainer);

				} else {
					DBG_PRINT_MSG("Warning: object %s is not found in container %s", widget->name().c_str(), name().c_str());
				}

			}

		}
	}

}
