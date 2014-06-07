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

		RemoveShadow(widget);
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

		RemoveShadow(widget);
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

		WidgetDeque::iterator it = m_sub_widgets->begin();
		std::advance(it, index);
		m_sub_widgets->insert(it, widget);

		RemoveShadow(widget);
		SetContainer(widget, this);

		events()->connect(widget->destroyed(), this,
						&AbstractDequeContainer::OnSubWidgetDestroyed);

		return true;
	}
	
	void AbstractDequeContainer::MoveSubWidgets (int offset_x, int offset_y)
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++) {
			SetSubWidgetPosition(*it, (*it)->position().x() + offset_x,
							(*it)->position().y() + offset_y);
		}
	}
	
	void AbstractDequeContainer::ResizeSubWidgets (const Size& size)
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin();
						it != m_sub_widgets->end(); it++) {
			ResizeSubWidget((*it), size);
		}
	}
	
	void AbstractDequeContainer::ResizeSubWidgets (int w,
					int h)
	{
		for (WidgetDeque::iterator it = sub_widgets()->begin();
						it != sub_widgets()->end(); it++) {
			ResizeSubWidget((*it), w, h);
		}
	}
	
	void AbstractDequeContainer::ClearSubWidgets ()
	{
		for (WidgetDeque::iterator it = m_sub_widgets->begin();
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

				for (WidgetDeque::const_iterator it = m_sub_widgets->begin();
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

				for (WidgetDeque::const_iterator it = m_sub_widgets->begin();
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

		for(WidgetDeque::const_iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
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
		for(WidgetDeque::const_iterator it = m_sub_widgets->begin(); it != m_sub_widgets->end(); it++)
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

	void AbstractDequeContainer::FillSubWidgetsInHBox (const Point& out_pos, const Size& out_size, const Margin& margin,
	        int alignment, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgetsProportionallyInHBox(x, y, width, height, alignment, space);
	}

	void AbstractDequeContainer::FillSubWidgetsInHBox (const Point& pos, const Size& size, int alignment, int space)
	{
		FillSubWidgetsProportionallyInHBox(pos.x(), pos.y(), size.width(), size.height(), alignment, space);
	}

	void AbstractDequeContainer::FillSubWidgetsProportionallyInHBox (int x, int y, int width,
					int height, int alignment, int space)
	{
		boost::scoped_ptr<std::deque<int> > expandable_preferred_widths(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_widths(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_heights(new std::deque<int>);

		int expandable_preferred_width_sum = 0;	// the width sum of the expandable widgets' size
		int unexpandable_preferred_width_sum = 0;	// the width sum of the unexpandable widgets' size

		AbstractWidget* widget = 0;
		Size tmp_size;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			widget = *it;
			if (widget->visiable()) {
				tmp_size = widget->GetPreferredSize();

				if(widget->IsExpandX()) {
					expandable_preferred_width_sum += tmp_size.width();
					expandable_preferred_widths->push_back(tmp_size.width());
				} else {
					unexpandable_preferred_width_sum += tmp_size.width();
					unexpandable_preferred_widths->push_back(tmp_size.width());
				}

				if(!widget->IsExpandY()) {
					unexpandable_preferred_heights->push_back(tmp_size.height());
				}

			}
		}

		if((expandable_preferred_widths->size() + unexpandable_preferred_widths->size()) == 0) return;	// do nothing if all sub widgets are invisible

		int total_space = ((expandable_preferred_widths->size() + unexpandable_preferred_widths->size()) - 1) * space;

		int total_preferred_width = expandable_preferred_width_sum
						+ unexpandable_preferred_width_sum
						+ total_space;

		if (total_preferred_width == width) {
			DistributeWithPreferredWidth(x, space,
							expandable_preferred_widths.get(),
							unexpandable_preferred_widths.get());
		} else if (total_preferred_width < width) {
			DistributeWithLargeWidth(x, width, space, expandable_preferred_widths.get(),
							expandable_preferred_width_sum, unexpandable_preferred_widths.get(),
							unexpandable_preferred_width_sum);
		} else {
			DistributeWithSmallWidth(x, width, space, expandable_preferred_widths.get(),
							expandable_preferred_width_sum, unexpandable_preferred_widths.get(),
							unexpandable_preferred_width_sum);
		}

		AlignInHBox(y, height, alignment, unexpandable_preferred_heights.get());
	}

	void AbstractDequeContainer::DistributeWithPreferredWidth (int x, int space,
					const std::deque<int>* expandable_preferred_widths,
					const std::deque<int>* unexpandable_preferred_widths)
	{
		std::deque<int>::const_iterator exp_it = expandable_preferred_widths->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_widths->begin();

		WidgetDeque::iterator widget_it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		while (widget_it != sub_widgets()->end()) {

			widget = *widget_it;

			if(widget->visiable()) {

				if(widget->IsExpandX()) {
					ResizeSubWidget(widget, *exp_it, widget->size().height());
					SetSubWidgetPosition(widget, x, widget->position().y());
					exp_it++;
				} else {
					ResizeSubWidget(widget, *unexp_it, widget->size().height());
					SetSubWidgetPosition(widget, x, widget->position().y());
					unexp_it++;
				}

				x = x + widget->size().width() + space;
			}

			widget_it++;
		}
	}

	void AbstractDequeContainer::DistributeWithSmallWidth (int x,
					int width,
					int space,
					const std::deque<int>* expandable_preferred_widths,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_widths,
					int unexpandable_prefer_sum)
	{
		int widgets_width = width - (expandable_preferred_widths->size() + unexpandable_preferred_widths->size() - 1) * space;

		if(widgets_width <= 0) {
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				(*it)->Resize(0, (*it)->size().height());
			}
			return;
		}

		int reference_width;
		std::deque<int>::const_iterator exp_it = expandable_preferred_widths->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_widths->begin();
		WidgetDeque::iterator it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		if(widgets_width <= unexpandable_prefer_sum) {
			reference_width = widgets_width;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->IsExpandX()) {
						ResizeSubWidget(widget, 0, widget->size().height());
						SetSubWidgetPosition(widget, x, widget->position().y());
						exp_it++;
					} else {
						ResizeSubWidget(widget,
										reference_width * (*unexp_it)
														/ unexpandable_prefer_sum,
										widget->size().height());
						SetSubWidgetPosition(widget, x, widget->position().y());
						unexp_it++;
					}

					x = x + widget->size().width() + space;
				}

				it++;
			}

		} else {
			reference_width = widgets_width - unexpandable_prefer_sum;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->IsExpandX()) {
						ResizeSubWidget(widget,
										reference_width * (*exp_it)
														/ expandable_prefer_sum,
										widget->size().height());
						SetSubWidgetPosition(widget, x, widget->position().y());
						exp_it++;
					} else {
						ResizeSubWidget(widget, (*unexp_it), widget->size().height());
						SetSubWidgetPosition(widget, x, widget->position().y());
						unexp_it++;
					}

					x = x + widget->size().width() + space;
				}

				it++;
			}

		}
	}

	void AbstractDequeContainer::DistributeWithLargeWidth (int x,
					int width,
					int space,
					const std::deque<int>* expandable_preferred_widths,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_widths,
					int unexpandable_prefer_sum)
	{
		int widgets_width = width - (expandable_preferred_widths->size() + unexpandable_preferred_widths->size() - 1) * space;

		int expandable_width = widgets_width - unexpandable_prefer_sum;

		std::deque<int>::const_iterator exp_it = expandable_preferred_widths->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_widths->begin();

		WidgetDeque::iterator it = sub_widgets()->begin();

		AbstractWidget* widget = 0;
		while (it != sub_widgets()->end()) {

			widget = (*it);

			if(widget->visiable()) {

				if (widget->IsExpandX()) {
					ResizeSubWidget(widget,
									expandable_width * (*exp_it)
													/ expandable_prefer_sum,
									widget->size().height());
					SetSubWidgetPosition(widget, x, widget->position().y());
					exp_it++;
				} else {
					ResizeSubWidget(widget, (*unexp_it), widget->size().height());
					SetSubWidgetPosition(widget, x, widget->position().y());
					unexp_it++;
				}

				x = x + widget->size().width() + space;
			}

			it++;
		}
	}

	void AbstractDequeContainer::AlignInHBox (int y, int height, int alignment, const std::deque<int>* unexpandable_preferred_heights)
	{
		std::deque<int>::const_iterator unexp_it =
		        unexpandable_preferred_heights->begin();
		WidgetDeque::iterator it;
		AbstractWidget* widget = 0;
		for (it = sub_widgets()->begin(); it != sub_widgets()->end(); it++) {
			widget = *it;

			if (widget->IsExpandY()) {

				ResizeSubWidget(widget, widget->size().width(), height);
				SetSubWidgetPosition(widget, widget->position().x(), y);

			} else {

				if ((*unexp_it) >= height) {
					ResizeSubWidget(widget, widget->size().width(), height);
					SetSubWidgetPosition(widget, widget->position().x(), y);
				} else {

					ResizeSubWidget(widget, widget->size().width(),
					        (*unexp_it));

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

				unexp_it++;
			}
		}
	}

	void AbstractDequeContainer::FillSubWidgetsInVBox (const Point& out_pos, const Size& out_size, const Margin& margin,
			int alignment, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgetsProportionallyInVBox(x, y, width, height, alignment, space);
	}

	void AbstractDequeContainer::FillSubWidgetsInVBox (const Point& pos, const Size& size, int alignment, int space)
	{
		FillSubWidgetsProportionallyInVBox(pos.x(), pos.y(), size.width(), size.height(), alignment, space);
	}

	void AbstractDequeContainer::FillSubWidgetsProportionallyInVBox (int x, int y, int width,
					int height, int alignment, int space)
	{
		boost::scoped_ptr<std::deque<int> > expandable_preferred_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_widths(new std::deque<int>);

		int expandable_preferred_height_sum = 0;	// the height sum of the expandable widgets' size
		int unexpandable_preferred_height_sum = 0;	// the height sum of the unexpandable widgets' size

		AbstractWidget* widget = 0;
		Size tmp_size;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			widget = *it;
			if (widget->visiable()) {
				tmp_size = widget->GetPreferredSize();

				if(widget->IsExpandY()) {
					expandable_preferred_height_sum += tmp_size.height();
					expandable_preferred_heights->push_back(tmp_size.height());
				} else {
					unexpandable_preferred_height_sum += tmp_size.height();
					unexpandable_preferred_heights->push_back(tmp_size.height());
				}

				if(!widget->IsExpandX()) {
					unexpandable_preferred_widths->push_back(tmp_size.width());
				}

			}
		}

		if((expandable_preferred_heights->size() + unexpandable_preferred_heights->size()) == 0) return;	// do nothing if all sub widgets are invisible

		int total_space = ((expandable_preferred_heights->size() + unexpandable_preferred_heights->size()) - 1) * space;

		int total_preferred_height = expandable_preferred_height_sum
						+ unexpandable_preferred_height_sum
						+ total_space;

		if (total_preferred_height == height) {
			DistributeWithPreferredHeight(y, height, space,
					expandable_preferred_heights.get(),
					unexpandable_preferred_heights.get());
		} else if (total_preferred_height < height) {
			DistributeWithLargeHeight(y, height, space, expandable_preferred_heights.get(),
							expandable_preferred_height_sum, unexpandable_preferred_heights.get(),
							unexpandable_preferred_height_sum);
		} else {
			DistributeWithSmallHeight(y, height, space, expandable_preferred_heights.get(),
							expandable_preferred_height_sum, unexpandable_preferred_heights.get(),
							unexpandable_preferred_height_sum);
		}

		AlignInVBox(x, width, alignment, unexpandable_preferred_widths.get());
	}

	void AbstractDequeContainer::DistributeWithPreferredHeight (int y,
					int height,
					int space,
					const std::deque<int>* expandable_preferred_heights,
					const std::deque<int>* unexpandable_preferred_heights)
	{
		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		WidgetDeque::iterator widget_it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		y = y + height;
		while (widget_it != sub_widgets()->end()) {

			widget = *widget_it;

			if(widget->visiable()) {

				if(widget->IsExpandY()) {
					ResizeSubWidget(widget, widget->size().width(), (*exp_it));
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(widget, widget->size().width(), (*unexp_it));
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					unexp_it++;
				}

				y = y - space;
			}

			widget_it++;
		}
	}

	void AbstractDequeContainer::DistributeWithSmallHeight (int y,
					int height,
					int space,
					const std::deque<int>* expandable_preferred_heights,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_heights,
					int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space;

		if(widgets_height <= 0) {
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				(*it)->Resize((*it)->size().width(), 0);
			}
			return;
		}

		int reference_height;
		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();
		WidgetDeque::iterator it = sub_widgets()->begin();
		AbstractWidget* widget = 0;

		y = y + height;
		if(widgets_height <= unexpandable_prefer_sum) {
			reference_height = widgets_height;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->IsExpandY()) {
						ResizeSubWidget(widget, widget->size().width(), 0);
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(widget,
										widget->size().width(),
										reference_height * (*unexp_it)
														/ unexpandable_prefer_sum
										);
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						unexp_it++;
					}

					y = y - space;
				}

				it++;
			}

		} else {
			reference_height = widgets_height - unexpandable_prefer_sum;

			while (it != sub_widgets()->end()) {

				widget = (*it);

				if(widget->visiable()) {

					if (widget->IsExpandY()) {
						ResizeSubWidget(widget,
										widget->size().width(),
										reference_height * (*exp_it)
														/ expandable_prefer_sum);
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						exp_it++;
					} else {
						ResizeSubWidget(widget, widget->size().width(), (*unexp_it));
						y = y - widget->size().height();
						SetSubWidgetPosition(widget, widget->position().x(), y);
						unexp_it++;
					}

					y = y - space;
				}

				it++;
			}

		}
	}

	void AbstractDequeContainer::DistributeWithLargeHeight (int y,
					int height,
					int space,
					const std::deque<int>* expandable_preferred_heights,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_heights,
					int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space;

		int expandable_height = widgets_height - unexpandable_prefer_sum;

		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		WidgetDeque::iterator it = sub_widgets()->begin();

		y = y + height;
		AbstractWidget* widget = 0;
		while (it != sub_widgets()->end()) {

			widget = (*it);

			if(widget->visiable()) {

				if (widget->IsExpandY()) {
					ResizeSubWidget(widget,
									widget->size().width(),
									expandable_height * (*exp_it)
													/ expandable_prefer_sum);
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					exp_it++;
				} else {
					ResizeSubWidget(widget, widget->size().width(), (*unexp_it));
					y = y - widget->size().height();
					SetSubWidgetPosition(widget, widget->position().x(), y);
					unexp_it++;
				}

				y = y - space;
			}

			it++;
		}
	}

	void AbstractDequeContainer::AlignInVBox (int x, int width, int alignment,
	        const std::deque<int>* unexpandable_preferred_widths)
	{
		std::deque<int>::const_iterator unexp_it =
				unexpandable_preferred_widths->begin();
		WidgetDeque::iterator it;
		AbstractWidget* widget = 0;
		for (it = sub_widgets()->begin(); it != sub_widgets()->end(); it++) {
			widget = *it;

			if (widget->IsExpandX()) {

				ResizeSubWidget(widget, width, widget->size().height());
				SetSubWidgetPosition(widget, x, widget->position().y());

			} else {

				if ((*unexp_it) >= width) {
					ResizeSubWidget(widget, width, widget->size().height());
					SetSubWidgetPosition(widget, x, widget->position().y());
				} else {

					ResizeSubWidget(widget, (*unexp_it),
					        widget->size().height());

					if (alignment & AlignLeft) {
						SetSubWidgetPosition(widget, x, widget->position().y());
					} else if (alignment & AlignRight) {
						SetSubWidgetPosition(widget,
						        x + (width - widget->size().width()),
						        widget->position().y());
					} else if (alignment & AlignVerticalCenter) {
						SetSubWidgetPosition(widget,
						        x + (width - widget->size().width()) / 2,
						        widget->position().y());
					}

				}

				unexp_it++;

			}
		}
	}

}

