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

	inline bool SubWidgetProxy::RequestSizeUpdateTest (
	        AbstractWidget* sub_widget, const SizeUpdateRequest& request)
	{
		if(sub_widget) {
			return sub_widget->SizeUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool SubWidgetProxy::RequestPositionUpdateTest (
	        AbstractWidget* sub_widget, const PositionUpdateRequest& request)
	{
		if(sub_widget) {
			return sub_widget->PositionUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool SubWidgetProxy::RequestRoundTypeUpdateTest (
	        AbstractWidget* sub_widget, const RoundTypeUpdateRequest& request)
	{
		if(sub_widget) {
			return sub_widget->RoundTypeUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool SubWidgetProxy::RequestRoundRadiusUpdateTest (
	        AbstractWidget* sub_widget, const RoundRadiusUpdateRequest& request)
	{
		if(sub_widget) {
			return sub_widget->RoundRadiusUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool SubWidgetProxy::RequestVisibilityUpdateTest (
	        AbstractWidget* sub_widget, const VisibilityUpdateRequest& request)
	{
		if(sub_widget) {
			return sub_widget->VisibilityUpdateTest(request);
		} else {
			return true;
		}
	}

	inline void SubWidgetProxy::RequestSizeUpdate (AbstractWidget* sub_widget,
	        const SizeUpdateRequest& request)
	{
		if(sub_widget) {
			sub_widget->PerformSizeUpdate(request);
		}
	}

	inline void SubWidgetProxy::RequestPositionUpdate (
	        AbstractWidget* sub_widget, const PositionUpdateRequest& request)
	{
		if(sub_widget) {
			sub_widget->PerformPositionUpdate(request);
		}
	}

	inline void SubWidgetProxy::RequestRoundTypeUpdate (
	        AbstractWidget* sub_widget, const RoundTypeUpdateRequest& request)
	{
		if(sub_widget) {
			sub_widget->PerformRoundTypeUpdate(request);
		}
	}

	inline void SubWidgetProxy::RequestRoundRadiusUpdate (
	        AbstractWidget* sub_widget, const RoundRadiusUpdateRequest& request)
	{
		if(sub_widget) {
			sub_widget->PerformRoundRadiusUpdate(request);
		}
	}

	inline void SubWidgetProxy::RequestVisibilityUpdate (
	        AbstractWidget* sub_widget, const VisibilityUpdateRequest& request)
	{
		if(sub_widget) {
			sub_widget->PerformVisibilityUpdate(request);
		}
	}

	void AbstractContainer::PerformMarginUpdate(const Margin& margin)
	{
	}

	void AbstractContainer::SetMargin (const Margin& margin)
	{
		if (m_margin.equal(margin))
			return;

		PerformMarginUpdate(margin);
		m_margin = margin;
	}

	void AbstractContainer::SetMargin (int left, int right, int top, int bottom)
	{
		if (m_margin.equal(left, right, top, bottom))
			return;

		Margin new_margin(left, right, top, bottom);
		PerformMarginUpdate(new_margin);
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
		SizeUpdateRequest request(this, sub, &new_size);

		if(SubWidgetProxy::RequestSizeUpdateTest(sub, request)) {
			SubWidgetProxy::RequestSizeUpdate(sub, request);
			sub->set_size(width, height);
		}
	}
	
	void AbstractContainer::ResizeSubWidget (AbstractWidget* sub,
	        const Size& size)
	{
		if (!sub || sub->container() != this)
			return;

		if (sub->size() == size)
			return;

		SizeUpdateRequest request(this, sub, &size);

		if (SubWidgetProxy::RequestSizeUpdateTest(sub, request)) {
			SubWidgetProxy::RequestSizeUpdate(sub, request);
			sub->set_size(size);
		}
	}
	
	void AbstractContainer::SetSubWidgetPosition (AbstractWidget* sub, int x,
	        int y)
	{
		if (!sub || sub->container() != this)
			return;

		if (sub->position().x() == x && sub->position().y() == y)
			return;

		Point new_pos(x, y);

		PositionUpdateRequest request(this, sub, &new_pos);

		if (SubWidgetProxy::RequestPositionUpdateTest(sub, request)) {
			SubWidgetProxy::RequestPositionUpdate(sub, request);
			sub->set_position(x, y);
		}
	}
	
	void AbstractContainer::SetSubWidgetPosition (AbstractWidget* sub,
					const Point& pos)
	{
		if(!sub || sub->container() != this) return;

		if(sub->position() == pos) return;

		PositionUpdateRequest request (this, sub, &pos);

		if(SubWidgetProxy::RequestPositionUpdateTest(sub, request)) {
			SubWidgetProxy::RequestPositionUpdate(sub, request);
			sub->set_position(pos);
		}
	}

	void AbstractContainer::SetSubWidgetVisibility (AbstractWidget* sub,
	        bool visible)
	{
		if(!sub || sub->container() != this) return;

		if(sub->visiable() == visible) return;

		VisibilityUpdateRequest request (this, sub, &visible);

		if(SubWidgetProxy::RequestVisibilityUpdateTest(sub, request)) {
			SubWidgetProxy::RequestVisibilityUpdate(sub, request);
			sub->set_visible(visible);
		}
	}

	bool AbstractContainer::EnableShadow(AbstractWidget* widget)
	{
		if(!widget) return false;
		if(widget->container() != this) return false;

		if(widget->drop_shadow()) {

			if(!widget->m_shadow) {
				widget->m_shadow.reset(new Shadow(widget->size(), widget->round_type(), widget->round_radius()));
			}

			widget->m_shadow->Update(widget->size(), widget->round_type(), widget->round_radius());

		} else {
			DBG_PRINT_MSG("The widget %s is not allow shadow by itself", widget->name().c_str());
			return false;
		}

		return true;
	}

	bool AbstractContainer::DisableShadow (AbstractWidget* widget)
	{
		if(!widget) return false;
		if(widget->container() != this) return false;

		widget->m_shadow.destroy();

		return true;
	}

}
