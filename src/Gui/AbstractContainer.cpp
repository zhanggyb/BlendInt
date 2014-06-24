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


	SingleIterator::SingleIterator (AbstractWidget* widget)
	: AbstractContainerIterator(), m_widget(widget), m_once(true)
	{
	}

	SingleIterator::SingleIterator (const SingleIterator& orig)
	: AbstractContainerIterator(), m_widget(0), m_once(true)
	{
		m_widget = orig.m_widget;
		m_once = orig.m_once;
	}

	SingleIterator::~SingleIterator ()
	{
	}

	SingleIterator& SingleIterator::operator = (const SingleIterator& orig)
	{
		m_widget = orig.m_widget;
		m_once = orig.m_once;
		return *this;
	}

	AbstractWidget* SingleIterator::GetWidget () const
	{
		return m_once ? m_widget : 0;
	}

	void SingleIterator::GoToFirst ()
	{
		m_once = true;
	}

	void SingleIterator::GoNext ()
	{
		m_once = false;
	}

	bool SingleIterator::IsEnd ()
	{
		if(m_widget == 0) {
			return true;
		} else {
			return !m_once;
		}
	}

	SubWidgetProxy::SubWidgetProxy ()
	{

	}

	SubWidgetProxy::~SubWidgetProxy ()
	{
	}

	inline bool SubWidgetProxy::RequestGeometryTest(AbstractWidget* sub_widget, const WidgetUpdateRequest& request)
	{
		if(sub_widget) {
			return sub_widget->UpdateGeometryTest(request);
		} else {
			return true;
		}
	}

	inline void SubWidgetProxy::RequestGeometryUpdate(AbstractWidget* sub_widget, const WidgetUpdateRequest& request)
	{
		if(sub_widget) {
			sub_widget->UpdateGeometry(request);
		}
	}

	bool AbstractContainer::UpdateGeometryTest (const WidgetUpdateRequest& request)
	{
		return true;
	}

	void AbstractContainer::BroadcastUpdate(const WidgetUpdateRequest& request)
	{
		// do nothing
	}

	void AbstractContainer::SetMargin (const Margin& margin)
	{
		if (m_margin.equal(margin))
			return;

		WidgetUpdateRequest request (this, this, ContainerMargin, &margin);

		UpdateContainer(request);
		m_margin = margin;
	}

	void AbstractContainer::SetMargin (int left, int right, int top, int bottom)
	{
		if (m_margin.equal(left, right, top, bottom))
			return;

		Margin new_margin(left, right, top, bottom);
		WidgetUpdateRequest request (this, this, ContainerMargin, &new_margin);

		UpdateContainer(request);
		m_margin = new_margin;
	}
	
	void AbstractContainer::ResizeSubWidget (AbstractWidget* sub,
					int width, int height)
	{
		if(!sub || sub->container() != this) return;

		if(sub->size().width() == width &&
				sub->size().height() == height)
			return;

		Size new_size (width, height);
		WidgetUpdateRequest request(this, sub, WidgetSize, &new_size);

		if(SubWidgetProxy::RequestGeometryTest(sub, request)) {
			SubWidgetProxy::RequestGeometryUpdate(sub, request);
			sub->set_size(width, height);
		}
	}
	
	void AbstractContainer::ResizeSubWidget (AbstractWidget* sub,
					const Size& size)
	{
		if(!sub || sub->container() != this) return;

		if(sub->size() == size) return;

		WidgetUpdateRequest request(this, sub, WidgetSize, &size);

		if(SubWidgetProxy::RequestGeometryTest(sub, request)) {
			SubWidgetProxy::RequestGeometryUpdate(sub, request);
			sub->set_size(size);
		}
	}
	
	void AbstractContainer::SetSubWidgetPosition (AbstractWidget* sub, int x,
					int y)
	{
		if(!sub || sub->container() != this) return;

		if(sub->position().x() == x &&
				sub->position().y() == y)
			return;

		Point new_pos (x, y);

		WidgetUpdateRequest request (this, sub, WidgetPosition, &new_pos);

		if(SubWidgetProxy::RequestGeometryTest(sub, request)) {
			SubWidgetProxy::RequestGeometryUpdate(sub, request);
			sub->set_position(x, y);
		}
	}
	
	void AbstractContainer::SetSubWidgetPosition (AbstractWidget* sub,
					const Point& pos)
	{
		if(!sub || sub->container() != this) return;

		if(sub->position() == pos) return;

		WidgetUpdateRequest request (this, sub, WidgetPosition, &pos);

		if(SubWidgetProxy::RequestGeometryTest(sub, request)) {
			SubWidgetProxy::RequestGeometryUpdate(sub, request);
			sub->set_position(pos);
		}
	}
	
	void AbstractContainer::RemoveShadow (AbstractWidget* widget)
	{
		widget->m_shadow.destroy();
	}

}
