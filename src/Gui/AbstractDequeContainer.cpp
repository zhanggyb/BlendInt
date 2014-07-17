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
		m_sub_widgets.reset(new AbstractWidgetDeque);
	}

	AbstractDequeContainer::~AbstractDequeContainer ()
	{
		ClearSubWidgets();
	}

	bool AbstractDequeContainer::PushFrontSubWidget (AbstractWidget* widget)
	{
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

		m_sub_widgets->push_front(widget);

		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractDequeContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractDequeContainer::PushBackSubWidget (AbstractWidget* widget)
	{
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

		m_sub_widgets->push_back(widget);

		SetContainer(widget, this);

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

		AbstractWidgetDeque::iterator it = std::find(m_sub_widgets->begin(),
						m_sub_widgets->end(), widget);

		if (it != m_sub_widgets->end()) {
			m_sub_widgets->erase(it);
			SetContainer(widget, 0);
		} else {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
							widget->name().c_str(), name().c_str());
			return false;
		}

		return true;
	}

	bool AbstractDequeContainer::FindSubWidget (AbstractWidget* widget)
	{
		AbstractWidgetDeque::iterator it = std::find(m_sub_widgets->begin(), m_sub_widgets->end(), widget);

		if(it != m_sub_widgets->end())
			return true;
		else
			return false;
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
				return false;
			} else {
				// Set widget's container to 0
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}

		}

		AbstractWidgetDeque::iterator it = m_sub_widgets->begin();
		std::advance(it, index);
		m_sub_widgets->insert(it, widget);

		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractDequeContainer::OnSubWidgetDestroyed);

		return true;
	}
	
	void AbstractDequeContainer::MoveSubWidgets (int offset_x, int offset_y)
	{
		for (AbstractWidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++) {
			SetSubWidgetPosition(*it, (*it)->position().x() + offset_x,
							(*it)->position().y() + offset_y);
		}
	}
	
	void AbstractDequeContainer::ResizeSubWidgets (const Size& size)
	{
		for (AbstractWidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++) {
			ResizeSubWidget((*it), size);
		}
	}
	
	void AbstractDequeContainer::ResizeSubWidgets (int w,
					int h)
	{
		for (AbstractWidgetDeque::iterator it = sub_widgets()->begin();
						it != sub_widgets()->end(); it++) {
			ResizeSubWidget((*it), w, h);
		}
	}
	
	void AbstractDequeContainer::ClearSubWidgets ()
	{
		for (AbstractWidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++)
		{
			(*it)->destroyed().disconnectOne(this,
							&AbstractDequeContainer::OnSubWidgetDestroyed);
			SetContainer(*it, 0);

			if ((*it)->managed() && ((*it)->count() == 0)) {
				delete *it;
			}
		}

		m_sub_widgets->clear();
	}

	IteratorPtr AbstractDequeContainer::CreateIterator (const DeviceEvent& event)
	{
		RefPtr<DequeIterator> iterator(new DequeIterator(m_sub_widgets.get()));

		return iterator;
	}
	
	ResponseType AbstractDequeContainer::FocusEvent (bool focus)
	{
		return Ignore;
	}

	void AbstractDequeContainer::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s",
						widget->name().c_str(), name().c_str());

		RemoveSubWidget(widget);
	}

	void AbstractDequeContainer::FillSubWidgetsAveragely (const Point& out_pos,
					const Size& out_size, const Margin& margin,
					Orientation orientation, int alignment, int space)
	{
		if(m_sub_widgets->empty()) return;

		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		if(orientation == Horizontal) {
			DistributeHorizontally(x, width, space);
			AlignHorizontally(y, height, alignment);
		} else {
			DistributeVertically(y, height, space);
			AlignVertically(x, width, alignment);
		}
	}

	void AbstractDequeContainer::FillSubWidgetsAveragely (const Point& pos, const Size& size,
					Orientation orientation, int alignment, int space)
	{
		if(m_sub_widgets->empty()) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(pos.x(), size.width(), space);
			AlignHorizontally(pos.y(), size.height(), alignment);
		} else {
			DistributeVertically(pos.y(), size.height(), space);
			AlignVertically(pos.x(), size.width(), alignment);
		}
	}

	void AbstractDequeContainer::FillSubWidgetsAveragely (int x, int y,
					int width, int height,
					Orientation orientation, int alignment, int space)
	{
		if(m_sub_widgets->empty()) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(x, width, space);
			AlignHorizontally(y, height, alignment);
		} else {
			DistributeVertically(y, height, space);
			AlignVertically(x, width, alignment);
		}
	}

	void AbstractDequeContainer::DistributeHorizontally (int x, int width, int space)
	{
		AbstractWidget* widget = 0;

		if (m_sub_widgets->size()) {
			int average_width = (width - ((m_sub_widgets->size() - 1)* space))
							/ m_sub_widgets->size();

			if (average_width > 0) {

				for (AbstractWidgetDeque::const_iterator it = m_sub_widgets->begin();
								it != m_sub_widgets->end(); it++) {
					widget = *it;
					ResizeSubWidget(widget, average_width, widget->size().height());
					SetSubWidgetPosition(widget, x, widget->position().y());
					x += average_width + space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}

	void AbstractDequeContainer::DistributeVertically (int y, int height, int space)
	{
		AbstractWidget* widget = 0;

		y = y + height;
		if (m_sub_widgets->size()) {
			int average_height = (height - ((m_sub_widgets->size() - 1)* space))
							/ m_sub_widgets->size();

			if (average_height > 0) {

				for (AbstractWidgetDeque::const_iterator it = m_sub_widgets->begin();
								it != m_sub_widgets->end(); it++) {
					widget = *it;
					ResizeSubWidget(widget, widget->size().width(), average_height);
					y -= average_height;
					SetSubWidgetPosition(widget, widget->position().x(), y);
					y -= space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}

	void AbstractDequeContainer::AlignHorizontally (int y, int height, int alignment)
	{
		AbstractWidget* widget = 0;

		for(AbstractWidgetDeque::const_iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
		{
			widget = *it;

			if(widget->IsExpandY()) {
				ResizeSubWidget(widget, widget->size().width(), height);
				SetSubWidgetPosition(widget, widget->position().x(), y);
			} else {

				if (alignment & AlignTop) {
					SetSubWidgetPosition(widget, widget->position().x(),
					        y + (height - widget->size().height()));
				} else if (alignment & AlignBottom) {
					SetSubWidgetPosition(widget, widget->position().x(), y);
				} else if (alignment & AlignHorizontalCenter) {
					SetSubWidgetPosition(widget, widget->position().x(),
					        y + (height - widget->size().height()) / 2);
				}

			}
		}
	}

	void AbstractDequeContainer::AlignVertically (int x, int width, int alignment)
	{
		AbstractWidget* widget = 0;
		for(AbstractWidgetDeque::const_iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
		{
			widget = *it;

			if (widget->IsExpandX()) {
				ResizeSubWidget(widget, width, widget->size().height());
				SetSubWidgetPosition(widget, x, widget->position().y());
			} else {

				if (alignment & AlignLeft) {
					SetSubWidgetPosition(widget, x, widget->position().y());
				} else if (alignment & AlignRight) {
					SetSubWidgetPosition(widget, x + (width - widget->size().width()), widget->position().y());
				} else if (alignment & AlignVerticalCenter) {
					SetSubWidgetPosition(widget, x + (width - widget->size().width()) / 2, widget->position().y());
				}

			}
		}
	}

}

