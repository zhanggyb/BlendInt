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

	AbstractContainer::AbstractContainer()
	: AbstractWidget(),
	  first_(0),
	  last_(0)
	{

	}

	AbstractContainer::AbstractContainer(size_t size)
	: AbstractWidget(),
	  first_(0),
	  last_(0)
	{
		m_deque.resize(size, 0);
	}

	AbstractContainer::~AbstractContainer()
	{
		Clear();

		ClearExt();
	}

	bool AbstractContainer::PushFrontSubWidget (AbstractWidget* widget)
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

		m_deque.push_front(widget);

		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::PushBackSubWidget (AbstractWidget* widget)
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

		m_deque.push_back(widget);

		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::AssignSubWidget (size_t index,
					AbstractWidget* widget)
	{
		if(!widget) return false;

		if((index + 1) > m_deque.size()) {
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

		// Remove orignal widget
		AbstractWidget* original = m_deque[index];
		if(original) {
			assert(original->container() == this);
			original->destroyed().disconnectOne(this,
					&AbstractContainer::OnSubWidgetDestroyed);
			SetContainer(original, 0);
		}

		AbstractWidgetDeque::iterator it = m_deque.begin();
		std::advance(it, index);

		*it = widget;

		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::InsertSubWidget (size_t index,
					AbstractWidget* widget)
	{
		if(!widget) return false;

		if(index > (m_deque.size() - 1)) {
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

		AbstractWidgetDeque::iterator it = m_deque.begin();
		std::advance(it, index);
		m_deque.insert(it, widget);

		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
						&AbstractContainer::OnSubWidgetDestroyed);

		AbstractWidgetDeque::iterator it = std::find(m_deque.begin(),
						m_deque.end(), widget);

		if (it != m_deque.end()) {
			m_deque.erase(it);
			SetContainer(widget, 0);
		} else {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
							widget->name().c_str(), name().c_str());
			return false;
		}

		return true;
	}

	bool AbstractContainer::FindSubWidget (AbstractWidget* widget)
	{
		AbstractWidgetDeque::iterator it = std::find(m_deque.begin(), m_deque.end(), widget);

		if(it != m_deque.end())
			return true;
		else
			return false;
	}

	void AbstractContainer::MoveSubWidgets (int offset_x, int offset_y)
	{
		for (AbstractWidgetDeque::iterator it = m_deque.begin();
						it != m_deque.end(); it++) {
			if(*it) {
				SetSubWidgetPosition(*it, (*it)->position().x() + offset_x,
								(*it)->position().y() + offset_y);
			}
		}
	}

	void AbstractContainer::ResizeSubWidgets (const Size& size)
	{
		for (AbstractWidgetDeque::iterator it = m_deque.begin();
						it != m_deque.end(); it++) {
			ResizeSubWidget((*it), size);
		}
	}

	void AbstractContainer::ResizeSubWidgets (int w,
					int h)
	{
		for (AbstractWidgetDeque::const_iterator it = deque().begin();
						it != deque().end(); it++) {
			ResizeSubWidget((*it), w, h);
		}
	}

	void AbstractContainer::Clear ()
	{
		for (AbstractWidgetDeque::iterator it = m_deque.begin();
						it != m_deque.end(); it++)
		{
			if(*it) {

				(*it)->destroyed().disconnectOne(this,
								&AbstractContainer::OnSubWidgetDestroyed);
				SetContainer(*it, 0);

				if ((*it)->managed() && ((*it)->count() == 0)) {
					delete *it;
				}

			}
		}

		m_deque.clear();
	}

	void AbstractContainer::ClearExt()
	{
		AbstractWidget* widget = first_;
		AbstractWidget* next = 0;

		while(widget) {

			next = widget->next_;

			//(*it)->destroyed().disconnectOne(this,
			//				&AbstractContainer::OnSubWidgetDestroyed);

			widget->previous_ = 0;
			widget->next_ = 0;
			widget->container_ = 0;

			if(widget->managed() && widget->count() == 0) {
				delete widget;
			}

			widget = next;
		}

		first_ = 0;
		last_ = 0;
	}

	ResponseType AbstractContainer::FocusEvent (bool focus)
	{
		return Ignore;
	}

	void AbstractContainer::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the container %s",
						widget->name().c_str(), name().c_str());

		RemoveSubWidget(widget);
	}

	void AbstractContainer::FillSubWidgetsAveragely (const Point& out_pos,
					const Size& out_size, const Margin& margin,
					Orientation orientation, int alignment, int space)
	{
		if(m_deque.empty()) return;

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

	void AbstractContainer::FillSubWidgetsAveragely (const Point& pos, const Size& size,
					Orientation orientation, int alignment, int space)
	{
		if(m_deque.empty()) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(pos.x(), size.width(), space);
			AlignHorizontally(pos.y(), size.height(), alignment);
		} else {
			DistributeVertically(pos.y(), size.height(), space);
			AlignVertically(pos.x(), size.width(), alignment);
		}
	}

	void AbstractContainer::FillSubWidgetsAveragely (int x, int y,
					int width, int height,
					Orientation orientation, int alignment, int space)
	{
		if(m_deque.empty()) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(x, width, space);
			AlignHorizontally(y, height, alignment);
		} else {
			DistributeVertically(y, height, space);
			AlignVertically(x, width, alignment);
		}
	}

	void AbstractContainer::DistributeHorizontally (int x, int width, int space)
	{
		AbstractWidget* widget = 0;

		if (m_deque.size()) {
			int average_width = (width - ((m_deque.size() - 1)* space))
							/ m_deque.size();

			if (average_width > 0) {

				for (AbstractWidgetDeque::const_iterator it = m_deque.begin();
								it != m_deque.end(); it++) {
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

	void AbstractContainer::DistributeVertically (int y, int height, int space)
	{
		AbstractWidget* widget = 0;

		y = y + height;
		if (m_deque.size()) {
			int average_height = (height - ((m_deque.size() - 1)* space))
							/ m_deque.size();

			if (average_height > 0) {

				for (AbstractWidgetDeque::const_iterator it = m_deque.begin();
								it != m_deque.end(); it++) {
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

	void AbstractContainer::AlignHorizontally (int y, int height, int alignment)
	{
		AbstractWidget* widget = 0;

		for(AbstractWidgetDeque::const_iterator it = m_deque.begin(); it != m_deque.end(); it++)
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

	bool AbstractContainer::PushFrontSubWidgetExt (AbstractWidget* widget)
	{
		if (!widget)
			return false;

		if (widget->container_) {

			if (widget->container_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name_.c_str(),
								widget->container_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				widget->container_->RemoveSubWidgetExt(widget);
			}

		}

		assert(widget->previous_ == 0);
		assert(widget->next_ == 0);
		assert(widget->container_ == 0);

		if(first_) {
			first_->previous_ = widget;
			widget->next_ = first_;
		} else {
			assert(last_ == 0);
			widget->next_ = 0;
			last_ = widget;
		}
		first_ = widget;

		widget->previous_ = 0;
		widget->container_ = this;

		//events()->connect(widget->destroyed(), this,
		//				&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::InsertSubWidgetExt (int index,
	        AbstractWidget* widget)
	{
		if (!widget)
			return false;

		if (widget->container_) {

			if (widget->container_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name_.c_str(),
								widget->container_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				widget->container_->RemoveSubWidgetExt(widget);
			}

		}

		assert(widget->previous_ == 0);
		assert(widget->next_ == 0);
		assert(widget->container_ == 0);

		if(first_ == 0) {
			assert(last_ == 0);

			widget->next_ = 0;
			last_ = widget;
			first_ = widget;
			widget->previous_ = 0;

		} else {

			AbstractWidget* p = first_;

			if(index > 0) {

				while(p && (index > 0)) {
					if(p->next_ == 0)
						break;

					p = p->next_;
					index--;
				}

				if(index == 0) {	// insert

					widget->previous_ = p->previous_;
					widget->next_ = p;
					p->previous_->next_ = widget;
					p->previous_ = widget;

				} else {	// same as push back

					assert(p == last_);
					last_->next_ = widget;
					widget->previous_ = last_;
					last_ = widget;
					widget->next_ = 0;

				}

			} else {	// same as push front

				first_->previous_ = widget;
				widget->next_ = first_;
				first_ = widget;
				widget->previous_ = 0;

			}

		}

		widget->container_ = this;

		//events()->connect(widget->destroyed(), this,
		//				&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::PushBackSubWidgetExt (AbstractWidget* widget)
	{
		if (!widget)
			return false;

		if (widget->container_) {

			if (widget->container_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name_.c_str(),
								widget->container_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				widget->container_->RemoveSubWidgetExt(widget);
			}

		}

		assert(widget->previous_ == 0);
		assert(widget->next_ == 0);
		assert(widget->container_ == 0);

		if(last_) {
			last_->next_ = widget;
			widget->previous_ = last_;
		} else {
			assert(first_ == 0);
			widget->previous_ = 0;
			first_ = widget;
		}
		last_ = widget;

		widget->next_ = 0;
		widget->container_ = this;

		//events()->connect(widget->destroyed(), this,
		//				&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractContainer::RemoveSubWidgetExt (AbstractWidget* widget)
	{
		if (!widget)
			return false;

		assert(widget->container_ == this);

		//widget->destroyed().disconnectOne(this,
		//        &AbstractContainer::OnSubWidgetDestroyed);

		if (widget->previous_) {
			widget->previous_->next_ = widget->next_;
		} else {
			assert(first_ == widget);
			first_ = widget->next_;
		}

		if (widget->next_) {
			widget->next_->previous_ = widget->previous_;
		} else {
			assert(last_ == widget);
			last_ = widget->previous_;
		}

		widget->previous_ = 0;
		widget->next_ = 0;
		widget->container_ = 0;

		return true;
	}

	int AbstractContainer::GetSubWidgetSize()
	{
		int sum = 0;

		AbstractWidget* p = first_;
		while(p) {
			sum++;
			p = p->next_;
		}

		return sum;
	}

	const AbstractWidget* AbstractContainer::operator [](int i) const
	{
		if(i < 0) return 0;

		AbstractWidget* widget = first_;

		while(widget && (i > 0)) {
			i--;
			widget = widget->next_;
		}

		if(i != 0) {	// out of range
			widget = 0;
		}

		return widget;
	}

	const AbstractWidget* AbstractContainer::GetWidgetAt(int i) const
	{
		if(i < 0) return 0;

		AbstractWidget* widget = first_;

		while(widget && (i > 0)) {
			i--;
			widget = widget->next_;
		}

		if(i != 0) {	// out of range
			widget = 0;
		}

		return widget;
	}

#ifdef DEBUG

	void AbstractContainer::ListSubWidgets()
	{
		AbstractWidget* widget = first_;
		while(widget) {
			DBG_PRINT_MSG("sub widget: %s", widget->name_.c_str());
			widget = widget->next_;
		}
	}

#endif

	void AbstractContainer::AlignVertically (int x, int width, int alignment)
	{
		AbstractWidget* widget = 0;
		for(AbstractWidgetDeque::const_iterator it = m_deque.begin(); it != m_deque.end(); it++)
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

		if(sub->SizeUpdateTest(request)) {
			sub->PerformSizeUpdate(request);
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

		if(sub->SizeUpdateTest(request)) {
			sub->PerformSizeUpdate(request);
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

		if(sub->PositionUpdateTest(request)) {
			sub->PerformPositionUpdate(request);
			sub->set_position(x, y);
		}
	}
	
	void AbstractContainer::SetSubWidgetPosition (AbstractWidget* sub,
					const Point& pos)
	{
		if(!sub || sub->container() != this) return;

		if(sub->position() == pos) return;

		PositionUpdateRequest request (this, sub, &pos);

		if(sub->PositionUpdateTest(request)) {
			sub->PerformPositionUpdate(request);
			sub->set_position(pos);
		}
	}

	void AbstractContainer::SetSubWidgetVisibility (AbstractWidget* sub,
	        bool visible)
	{
		if(!sub || sub->container() != this) return;

		if(sub->visiable() == visible) return;

		VisibilityUpdateRequest request (this, sub, &visible);

		if(sub->VisibilityUpdateTest(request)) {
			sub->PerformVisibilityUpdate(request);
			sub->set_visible(visible);
		}
	}

	bool AbstractContainer::EnableShadow(AbstractWidget* widget)
	{
		if(!widget) return false;
		if(widget->container() != this) return false;

		if(widget->drop_shadow()) {

			if(!widget->shadow_) {
				widget->shadow_.reset(new Shadow(widget->size(), widget->round_type(), widget->round_radius()));
			}

			widget->shadow_->Update(widget->size(), widget->round_type(), widget->round_radius());

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

		widget->shadow_.destroy();

		return true;
	}

	void AbstractContainer::FillSingleWidget (size_t index, const Point& out_pos,
	        const Size& out_size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();

		int w = out_size.width() - margin.left()
							- margin.right();
		int h = out_size.height() - margin.top()
							- margin.bottom();

		FillSingleWidget(index, x, y, w, h);
	}

	void AbstractContainer::FillSingleWidget (size_t index, const Point& pos,
	        const Size& size)
	{
		FillSingleWidget(index, pos.x(), pos.y(), size.width(), size.height());
	}

	void AbstractContainer::FillSingleWidget (size_t index, int left, int bottom,
	        int width, int height)
	{
		AbstractWidget* widget = m_deque[index];

		if (widget) {
			ResizeSubWidget(widget, width, height);
			SetSubWidgetPosition(widget, left, bottom);

			if (widget->size().width() < width) {
				SetSubWidgetPosition(widget,
				        left + (width - widget->size().width()) / 2, bottom);
			}

			if (widget->size().height() < height) {
				SetSubWidgetPosition(widget, left,
				        bottom + (height - widget->size().height() / 2));
			}
		}
	}

}
