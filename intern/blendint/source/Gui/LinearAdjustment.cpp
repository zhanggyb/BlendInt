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

#include <BlendInt/Gui/LinearAdjustment.hpp>

namespace BlendInt {

	LinearAdjustment::LinearAdjustment(AbstractView* view, Orientation orient, int alignment, int space)
	: AbstractAdjustment(view),
	  orientation_(orient),
	  alignment_(alignment),
	  space_(space)
	{

	}

	LinearAdjustment::~LinearAdjustment()
	{

	}

	void LinearAdjustment::Adjust (int x, int y, int w, int h)
	{
		if(orientation_ == Horizontal) {

			AdjustHorizontally(x, y, w, h);

		} else {

		}
	}

	void LinearAdjustment::AdjustHorizontally(int x, int y, int w, int h)
	{
		int expandable_preferred_width_sum = 0;	// the width sum of the expandable widgets' size
		int unexpandable_preferred_width_sum = 0;	// the width sum of the unexpandable widgets' size

		Size tmp_size;
		for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
		{
			if (p->visiable()) {
				tmp_size = p->GetPreferredSize();

				if(p->IsExpandX()) {
					expandable_preferred_width_sum += tmp_size.width();
					expandable_preferred_width_list_.push_back(tmp_size.width());
				} else {
					unexpandable_preferred_width_sum += tmp_size.width();
					unexpandable_preferred_width_list_.push_back(tmp_size.width());
				}

				if(!p->IsExpandY()) {
					unexpandable_preferred_height_list_.push_back(tmp_size.height());
				}
			}
		}

		if ((expandable_preferred_width_list_.size()
		        + unexpandable_preferred_width_list_.size()) == 0)
			return;	// do nothing if all sub widgets are invisible

		int total_space = ((expandable_preferred_width_list_.size() +
				unexpandable_preferred_width_list_.size()) - 1) * space_;

		int total_preferred_width = expandable_preferred_width_sum
						+ unexpandable_preferred_width_sum
						+ total_space;

		if (total_preferred_width == w) {
			DistributeWithPreferredWidth(x);
		} else if (total_preferred_width < w) {
			DistributeWithLargeWidth(x, w,
			        expandable_preferred_width_sum,
			        unexpandable_preferred_width_sum);
		} else {
			DistributeWithSmallWidth(x,
					w,
					expandable_preferred_width_sum,
					unexpandable_preferred_width_sum);
		}

		AlignHorizontally(y, h);

	}

	void LinearAdjustment::DistributeWithPreferredWidth (int x)
	{
		std::deque<int>::const_iterator exp_it = expandable_preferred_width_list_.begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_width_list_.begin();

		AbstractView* p = view()->first_subview();

		while (p) {

			if(p->visiable()) {

				if(p->IsExpandX()) {
					resize(p, *exp_it, p->size().height());
					move(p, x, p->position().y());
					exp_it++;
				} else {
					resize(p, *unexp_it, p->size().height());
					move(p, x, p->position().y());
					unexp_it++;
				}

				x = x + p->size().width() + space_;
			}

			p = p->next_view();
		}

	}

	void LinearAdjustment::DistributeWithSmallWidth (int x, int width,
	        int expandable_prefer_sum,
	        int unexpandable_prefer_sum)
	{
		int widgets_width = width - (expandable_preferred_width_list_.size() +
				unexpandable_preferred_width_list_.size() - 1) * space_;

		if(widgets_width <= 0) {
			for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
			{
				p->Resize(0, p->size().height());
			}
			return;
		}

		int reference_width;
		std::deque<int>::const_iterator exp_it = expandable_preferred_width_list_.begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_width_list_.begin();

		AbstractView* p = view()->first_subview();

		if(widgets_width <= unexpandable_prefer_sum) {
			reference_width = widgets_width;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandX()) {
						resize(p, 0, p->size().height());
						move(p, x, p->position().y());
						exp_it++;
					} else {
						resize(p,
						        reference_width * (*unexp_it)
						                / unexpandable_prefer_sum,
						        p->size().height());
						move(p, x, p->position().y());
						unexp_it++;
					}

					x = x + p->size().width() + space_;
				}

				p = p->next_view();
			}

		} else {
			reference_width = widgets_width - unexpandable_prefer_sum;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandX()) {
						resize(p,
						        reference_width * (*exp_it)
						                / expandable_prefer_sum,
						        p->size().height());
						move(p, x, p->position().y());
						exp_it++;
					} else {
						resize(p, (*unexp_it), p->size().height());
						move(p, x, p->position().y());
						unexp_it++;
					}

					x = x + p->size().width() + space_;
				}

				p = p->next_view();
			}

		}
	}

	void LinearAdjustment::DistributeWithLargeWidth (int x, int width,
	        int expandable_prefer_sum, int unexpandable_prefer_sum)
	{
		int widgets_width = width - (expandable_preferred_width_list_.size() +
				unexpandable_preferred_width_list_.size() - 1) * space_;

		int expandable_width = widgets_width - unexpandable_prefer_sum;

		std::deque<int>::const_iterator exp_it = expandable_preferred_width_list_.begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_width_list_.begin();

		AbstractView* p = view()->first_subview();
		while (p) {

			if(p->visiable()) {

				if (p->IsExpandX()) {
					resize(p,
									expandable_width * (*exp_it)
													/ expandable_prefer_sum,
									p->size().height());
					move(p, x, p->position().y());
					exp_it++;
				} else {
					resize(p, (*unexp_it), p->size().height());
					move(p, x, p->position().y());
					unexp_it++;
				}

				x = x + p->size().width() + space_;
			}

			p = p->next_view();
		}
	}

	void LinearAdjustment::AlignHorizontally (int y, int height)
	{
		std::deque<int>::const_iterator unexp_it =
		        unexpandable_preferred_height_list_.begin();

		for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
		{
			if (p->IsExpandY()) {

				resize(p, p->size().width(), height);
				move(p, p->position().x(), y);

			} else {

				if ((*unexp_it) >= height) {
					resize(p, p->size().width(), height);
					move(p, p->position().x(), y);
				} else {

					resize(p, p->size().width(),
					        (*unexp_it));

					if (alignment_ & AlignTop) {
						move(p, p->position().x(),
						        y + (height - p->size().height()));
					} else if (alignment_ & AlignBottom) {
						move(p, p->position().x(), y);
					} else if (alignment_ & AlignHorizontalCenter) {
						move(p, p->position().x(),
						        y + (height - p->size().height()) / 2);
					}
				}

				unexp_it++;
			}
		}

	}

}
