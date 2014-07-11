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

	bool AbstractSingleContainer::IsExpandX () const
	{
		if(m_sub_widget) {
			return m_sub_widget->IsExpandX();
		} else {
			return false;
		}
	}

	bool AbstractSingleContainer::IsExpandY () const
	{
		if(m_sub_widget) {
			return m_sub_widget->IsExpandY();
		} else {
			return false;
		}
	}

	Size AbstractSingleContainer::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget()) {
			preferred_size = sub_widget()->GetPreferredSize();
			preferred_size.add_width(margin().left() + margin().right());
			preferred_size.add_height(margin().top() + margin().bottom());
		} else {
			preferred_size.set_width(200);
			preferred_size.set_height(200);
		}

		return preferred_size;
	}

	void AbstractSingleContainer::FillSubWidget (const Point& pos, const Size& out_size, const Margin& margin)
	{
		if (m_sub_widget) {
			int x = pos.x() + margin.left();
			int y = pos.y() + margin.bottom();

			int w = out_size.width() - margin.left()
							- margin.right();
			int h = out_size.height() - margin.top()
							- margin.bottom();

			FillSubWidget(x, y, w, h);
		}
	}

	void AbstractSingleContainer::FillSubWidget (const Point& pos, const Size& size)
	{
		if (m_sub_widget) {
			ResizeSubWidget(m_sub_widget, size.width(), size.height());
			SetSubWidgetPosition(m_sub_widget, pos.x(), pos.y());

			if (m_sub_widget->size().width() < size.width()) {
				SetSubWidgetPosition(m_sub_widget,
								pos.x() + (size.width() - m_sub_widget->size().width()) / 2, pos.y());
			}

			if (m_sub_widget->size().height() < size.height()) {
				SetSubWidgetPosition(m_sub_widget, pos.x(),
								pos.y() + (size.height() - m_sub_widget->size().height() / 2));
			}
		}
	}

	void AbstractSingleContainer::FillSubWidget (int x, int y, int w, int h)
	{
		if (m_sub_widget) {
			ResizeSubWidget(m_sub_widget, w, h);
			SetSubWidgetPosition(m_sub_widget, x, y);

			if (m_sub_widget->size().width() < w) {
				SetSubWidgetPosition(m_sub_widget,
								x + (w - m_sub_widget->size().width()) / 2, y);
			}

			if (m_sub_widget->size().height() < h) {
				SetSubWidgetPosition(m_sub_widget, x,
								y + (h - m_sub_widget->size().height() / 2));
			}
		}
	}
	
	bool AbstractSingleContainer::SetSubWidget (AbstractWidget* widget)
	{
		if(!widget)
			return false;

		if(widget->container()) {

			if(widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
									widget->name().c_str(),
									widget->container()->name().c_str());
				return false;
			} else {
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}
		}

		m_sub_widget = widget;

		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractSingleContainer::OnSubWidgetDestroyed);

		CheckSubWidgetAddedInContainer(widget);

		return true;
	}
	
	bool AbstractSingleContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;

		if(widget->container() != this) return false;

		widget->destroyed().disconnectOne(this,
						&AbstractSingleContainer::OnSubWidgetDestroyed);

		m_sub_widget = 0;
		SetContainer(widget, 0);

		CheckSubWidgetRemovedInContainer(widget);

		return true;
	}
	
	IteratorPtr AbstractSingleContainer::CreateIterator (
					const DeviceEvent& event)
	{
		RefPtr<SingleIterator> ret(new SingleIterator(m_sub_widget));

		return ret;
	}
	
	ResponseType AbstractSingleContainer::FocusEvent (bool focus)
	{
		return Ignore;
	}

	void AbstractSingleContainer::OnSubWidgetDestroyed(AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s",
						widget->name().c_str(), name().c_str());

		RemoveSubWidget(widget);
	}

	void AbstractSingleContainer::MoveSubWidget (int offset_x, int offset_y)
	{
		if(m_sub_widget) {
			SetSubWidgetPosition(m_sub_widget,
							m_sub_widget->position().x() + offset_x,
							m_sub_widget->position().y() + offset_y);
		}
	}

}
