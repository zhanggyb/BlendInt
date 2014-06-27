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

#include <BlendInt/Gui/AbstractVectorContainer.hpp>

namespace BlendInt {

	AbstractVectorContainer::AbstractVectorContainer (size_t num)
	: AbstractContainer()
	{
		m_sub_widgets.reset(new WidgetVector(num, 0));
	}

	AbstractVectorContainer::~AbstractVectorContainer()
	{
		ClearSubWidgets();
	}

	bool AbstractVectorContainer::SetSubWidget(size_t index, AbstractWidget* widget)
	{
		if(index >= m_sub_widgets->size())
			return false;

		if (!widget)
			return false;

		if (widget->container()) {

			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name().c_str(),
								widget->container()->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}

		}

		if(m_sub_widgets->operator [](index)) {
			RemoveSubWidget(m_sub_widgets->operator [](index));
		}

		m_sub_widgets->operator [](index) = widget;

		RemoveShadow(widget);
		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractVectorContainer::OnSubWidgetDestroyed);

		CheckSubWidgetAddedInContainer(widget);

		return true;
	}

	ResponseType AbstractVectorContainer::FocusEvent(bool focus)
	{
		return Ignore;
	}

	bool AbstractVectorContainer::RemoveSubWidget(AbstractWidget* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
						&AbstractVectorContainer::OnSubWidgetDestroyed);

		WidgetVector::iterator it = std::find(m_sub_widgets->begin(),
						m_sub_widgets->end(), widget);

		if (it != m_sub_widgets->end()) {
			*it = 0;
			SetContainer(widget, 0);
		} else {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
							widget->name().c_str(), name().c_str());
			return false;
		}

		CheckSubWidgetRemovedInContainer(widget);

		return true;
	}

	IteratorPtr AbstractVectorContainer::CreateIterator (
	        const DeviceEvent& event)
	{
		RefPtr<VectorIterator> iterator(new VectorIterator(m_sub_widgets.get()));

		return iterator;
	}

	void AbstractVectorContainer::MoveSubWidgets (int offset_x, int offset_y)
	{
		AbstractWidget* widget = 0;
		for (WidgetVector::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++)
		{
			widget = *it;

			if(widget) {
				SetSubWidgetPosition(widget, widget->position().x() + offset_x,
						widget->position().y() + offset_y);
			}
		}
	}

	void AbstractVectorContainer::ClearSubWidgets ()
	{
		AbstractWidget* widget = 0;
		for (WidgetVector::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++)
		{
			widget = *it;

			if(widget) {
				widget->destroyed().disconnectOne(this,
								&AbstractVectorContainer::OnSubWidgetDestroyed);
				SetContainer(widget, 0);

				if (widget->managed() && (widget->count() == 0)) {
					delete widget;
				}
			}
		}

		m_sub_widgets->clear();
	}

	void AbstractVectorContainer::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s",
						widget->name().c_str(), name().c_str());

		RemoveSubWidget(widget);
	}

}
