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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <boost/smart_ptr.hpp>
#include <BlendInt/Gui/VLayout.hpp>

namespace BlendInt {

	VLayout::VLayout (int align, int space)
	: AbstractLayout(), alignment_(align), space_(space)
	{
		set_size (200, 200);
	}

	VLayout::~VLayout ()
	{
	}

	bool VLayout::AddWidget(AbstractWidget* widget)
	{
		if(PushBackSubView(widget)) {
			UpdateLayout();
			RequestRedraw();

			return true;
		}

		return false;
	}

	bool VLayout::InsertWidget(int index, AbstractWidget* widget)
	{
		if(InsertSubView(index, widget)) {
			UpdateLayout();
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool VLayout::InsertWidget(int row, int column,
			AbstractWidget* widget)
	{
		if(column != 0) {
			DBG_PRINT_MSG("Error: %s",
						  "HLayout contains only 1 row, and the 1st parameter will be ignored");
		}

		if(InsertSubView(row, widget)) {
			UpdateLayout();
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool VLayout::Remove (AbstractWidget* widget)
	{
		if(RemoveSubView(widget)) {
			UpdateLayout();
			return true;
		}

		return false;
	}

	void VLayout::SetAlignment (int align)
	{
		if(alignment_ == align) return;

		alignment_ = align;
		UpdateLayout();
		RequestRedraw();
	}

	void VLayout::SetSpace (int space)
	{
		if(space_ == space) return;

		space_ = space;
		UpdateLayout();
		RequestRedraw();
	}

	Size BlendInt::VLayout::GetPreferredSize () const
	{
		Size preferred_size;

		if(first_subview() == 0) {

			preferred_size.set_width(200);
			preferred_size.set_height(200);

		} else {

			Size tmp_size;

			preferred_size.set_height(-space_);
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					preferred_size.set_width(std::max(preferred_size.width(), tmp_size.width()));
					preferred_size.add_height(tmp_size.height() + space_);
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	bool VLayout::IsExpandX () const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool VLayout::IsExpandY () const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	void VLayout::PerformMarginUpdate(const Margin& request)
	{
		set_margin(request);
		UpdateLayout();
		RequestRedraw();
	}

	bool VLayout::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		// Do not allow sub widget changing its size
		if(request.source()->superview() == this) {
			return false;
		}

		return true;
	}

	bool VLayout::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		// Do not allow sub widget changing its position
		if(request.source()->superview() == this) {
			return false;
		}

		return true;
	}

	void VLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			UpdateLayout();
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void VLayout::UpdateLayout ()
	{
		int x = margin().left();
		int y = margin().bottom();
		int width = size().width() - margin().hsum();
		int height = size().height() - margin().vsum();

		boost::scoped_ptr<std::deque<int> > expandable_preferred_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_preferred_widths(new std::deque<int>);

		int expandable_preferred_height_sum = 0;	// the height sum of the expandable widgets' size
		int unexpandable_preferred_height_sum = 0;	// the height sum of the unexpandable widgets' size

		Size tmp_size;
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if (p->visiable()) {
				tmp_size = p->GetPreferredSize();

				if(p->IsExpandY()) {
					expandable_preferred_height_sum += tmp_size.height();
					expandable_preferred_heights->push_back(tmp_size.height());
				} else {
					unexpandable_preferred_height_sum += tmp_size.height();
					unexpandable_preferred_heights->push_back(tmp_size.height());
				}

				if(!p->IsExpandX()) {
					unexpandable_preferred_widths->push_back(tmp_size.width());
				}

			}
		}

		if ((expandable_preferred_heights->size()
		        + unexpandable_preferred_heights->size()) == 0)
			return;	// do nothing if all sub widgets are invisible

		int total_space = ((expandable_preferred_heights->size()
		        + unexpandable_preferred_heights->size()) - 1) * space_;

		int total_preferred_height = expandable_preferred_height_sum
						+ unexpandable_preferred_height_sum
						+ total_space;

		if (total_preferred_height == height) {
			DistributeWithPreferredHeight(y, height,
			        expandable_preferred_heights.get(),
			        unexpandable_preferred_heights.get());
		} else if (total_preferred_height < height) {
			DistributeWithLargeHeight(y, height,
			        expandable_preferred_heights.get(),
			        expandable_preferred_height_sum,
			        unexpandable_preferred_heights.get(),
			        unexpandable_preferred_height_sum);
		} else {
			DistributeWithSmallHeight(y, height,
			        expandable_preferred_heights.get(),
			        expandable_preferred_height_sum,
			        unexpandable_preferred_heights.get(),
			        unexpandable_preferred_height_sum);
		}

		AlignInVBox(x, width, unexpandable_preferred_widths.get());
	}

	void VLayout::DistributeWithPreferredHeight (int y,
					int height,
					const std::deque<int>* expandable_preferred_heights,
					const std::deque<int>* unexpandable_preferred_heights)
	{
		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		AbstractView* p = first_subview();

		y = y + height;
		while (p) {

			if(p->visiable()) {

				if(p->IsExpandY()) {
					ResizeSubView(p, p->size().width(), (*exp_it));
					y = y - p->size().height();
					MoveSubViewTo(p, p->position().x(), y);
					exp_it++;
				} else {
					ResizeSubView(p, p->size().width(), (*unexp_it));
					y = y - p->size().height();
					MoveSubViewTo(p, p->position().x(), y);
					unexp_it++;
				}

				y = y - space_;
			}

			p = p->next_view();
		}
	}

	void VLayout::DistributeWithSmallHeight (int y,
					int height,
					const std::deque<int>* expandable_preferred_heights,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_heights,
					int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space_;

		if(widgets_height <= 0) {
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				p->Resize(p->size().width(), 0);
			}
			return;
		}

		int reference_height;
		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		AbstractView* p = first_subview();

		y = y + height;
		if(widgets_height <= unexpandable_prefer_sum) {
			reference_height = widgets_height;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandY()) {
						ResizeSubView(p, p->size().width(), 0);
						y = y - p->size().height();
						MoveSubViewTo(p, p->position().x(), y);
						exp_it++;
					} else {
						ResizeSubView(p,
										p->size().width(),
										reference_height * (*unexp_it)
														/ unexpandable_prefer_sum
										);
						y = y - p->size().height();
						MoveSubViewTo(p, p->position().x(), y);
						unexp_it++;
					}

					y = y - space_;
				}

				p = p->next_view();
			}

		} else {
			reference_height = widgets_height - unexpandable_prefer_sum;

			while (p) {

				if(p->visiable()) {

					if (p->IsExpandY()) {
						ResizeSubView(p,
										p->size().width(),
										reference_height * (*exp_it)
														/ expandable_prefer_sum);
						y = y - p->size().height();
						MoveSubViewTo(p, p->position().x(), y);
						exp_it++;
					} else {
						ResizeSubView(p, p->size().width(), (*unexp_it));
						y = y - p->size().height();
						MoveSubViewTo(p, p->position().x(), y);
						unexp_it++;
					}

					y = y - space_;
				}

				p = p->next_view();
			}

		}
	}

	void VLayout::DistributeWithLargeHeight (int y,
					int height,
					const std::deque<int>* expandable_preferred_heights,
					int expandable_prefer_sum,
					const std::deque<int>* unexpandable_preferred_heights,
					int unexpandable_prefer_sum)
	{
		int widgets_height = height - (expandable_preferred_heights->size() + unexpandable_preferred_heights->size() - 1) * space_;

		int expandable_height = widgets_height - unexpandable_prefer_sum;

		std::deque<int>::const_iterator exp_it = expandable_preferred_heights->begin();
		std::deque<int>::const_iterator unexp_it = unexpandable_preferred_heights->begin();

		AbstractView* p = first_subview();

		y = y + height;
		while (p) {

			if(p->visiable()) {

				if (p->IsExpandY()) {
					ResizeSubView(p,
									p->size().width(),
									expandable_height * (*exp_it)
													/ expandable_prefer_sum);
					y = y - p->size().height();
					MoveSubViewTo(p, p->position().x(), y);
					exp_it++;
				} else {
					ResizeSubView(p, p->size().width(), (*unexp_it));
					y = y - p->size().height();
					MoveSubViewTo(p, p->position().x(), y);
					unexp_it++;
				}

				y = y - space_;
			}

			p = p->next_view();
		}
	}

	void VLayout::AlignInVBox (int x, int width,
	        const std::deque<int>* unexpandable_preferred_widths)
	{
		std::deque<int>::const_iterator unexp_it =
				unexpandable_preferred_widths->begin();

		for (AbstractView* p = first_subview(); p; p = p->next_view()) {

			if (p->IsExpandX()) {

				ResizeSubView(p, width, p->size().height());
				MoveSubViewTo(p, x, p->position().y());

			} else {

				if ((*unexp_it) >= width) {
					ResizeSubView(p, width, p->size().height());
					MoveSubViewTo(p, x, p->position().y());
				} else {

					ResizeSubView(p, (*unexp_it),
					        p->size().height());

					if (alignment_ & AlignLeft) {
						MoveSubViewTo(p, x, p->position().y());
					} else if (alignment_ & AlignRight) {
						MoveSubViewTo(p,
						        x + (width - p->size().width()),
						        p->position().y());
					} else if (alignment_ & AlignVerticalCenter) {
						MoveSubViewTo(p,
						        x + (width - p->size().width()) / 2,
						        p->position().y());
					}

				}

				unexp_it++;

			}
		}
	}


}
