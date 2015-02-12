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

#include <cassert>

#include <core/types.hpp>
#include <gui/table-adjustment.hpp>

namespace BlendInt {

	TableAdjustment::TableAdjustment(AbstractView* view, unsigned int row, unsigned int column, int space)
	: AbstractAdjustment(view),
	  row_(row),
	  column_(column),
	  space_(space),
	  total_fixed_column_width_(0),
	  total_fixed_row_height_(0),
	  fixed_column_num_(0),
	  fixed_row_num_(0),
	  expand_x_(false),
	  expand_y_(false)
	{
		assert(view->subs_count() == (int)(row * column));
	}

	TableAdjustment::~TableAdjustment ()
	{
	}

	void TableAdjustment::Adjust (int x, int y, int w, int h)
	{
		column_expand_status_.resize(column_, false);
		row_expand_status_.resize(row_, false);

		column_width_list_.resize(column_, 0);
		row_height_list_.resize(row_, 0);

		total_fixed_column_width_ = 0;
		total_fixed_row_height_ = 0;
		fixed_column_num_ = 0;
		fixed_row_num_ = 0;
		expand_x_ = false;
		expand_y_ = false;

		Scan ();
		Size total_preferred_size = GetTotalPreferredSize();

		if(w < total_preferred_size.width()) {
			DistributeHorizontallyWithSmallWidth(x, w);
		} else if (w > total_preferred_size.width()) {
			DistributeHorizontallyWithLargeWidth(x, w);
		} else {
			DistributeHorizontallyWithPreferredWidth(x, w);
		}

		if(h < total_preferred_size.height()) {
			DistributeVerticallyWithSmallHeight(y, h);
		} else if (h > total_preferred_size.height()) {
			DistributeVerticallyWithLargeHeight(y, h);
		} else {
			DistributeVerticallyWithPreferredHeight(y, h);
		}
	}

	void TableAdjustment::Scan()
	{
		unsigned int i = 0;	// row
		unsigned int j = 0;	// column
		Size tmp;

		for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
		{
			tmp = p->GetPreferredSize();

			column_width_list_[j] = std::max(tmp.width(), column_width_list_[j]);
			row_height_list_[i] = std::max(tmp.height(), row_height_list_[i]);

			if(p->IsExpandX()) {
				column_expand_status_[j] = true;
				expand_x_ = true;
			}

			if(p->IsExpandY()) {
				row_expand_status_[i] = true;
				expand_y_ = true;
			}

			j++;
			if(j == column_) {
				j = 0;
				i++;
			}
		}

		for(j = 0; j < column_; j++) {
			if(!column_expand_status_[j]) {
				fixed_column_num_++;
				total_fixed_column_width_ += column_width_list_[j];
			}
		}

		for(i = 0; i < row_; i++) {
			if(!row_expand_status_[i]) {
				fixed_row_num_++;
				total_fixed_row_height_ += row_height_list_[i];
			}
		}
	}

	Size TableAdjustment::GetTotalPreferredSize() const
	{
		int total_width = 0;
		int total_height = 0;

		if(column_ > 0) {
			total_width += (column_ - 1) * space_;
		}

		if(row_ > 0) {
			total_height += (row_ - 1) * space_;
		}

		for(size_t i = 0; i < column_; i++)
		{
			total_width += column_width_list_[i];
		}

		for(size_t i = 0; i < row_; i++)
		{
			total_height += row_height_list_[i];
		}

		return Size(total_width, total_height);
	}

	void TableAdjustment::DistributeHorizontallyWithSmallWidth (int x, int width) const
	{
		int valid_width = width - (column_ - 1) * space_;

		unsigned int i = 0;	// row
		unsigned int j = 0;	// column

		int xpos = x;
		if(valid_width <= 0) {
			for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
			{
				resize(p, 0, p->size().height());
				move(p, xpos, p->position().y());

				j++;
				if(j == column_) {
					j = 0;
					i++;
				}
			}

			return;
		}

		if(expand_x_) {

			assert(fixed_column_num_ != (int)column_);

			int average_expandable_width = (valid_width - total_fixed_column_width_) / (column_ - fixed_column_num_);

			if(average_expandable_width > 0) {

				for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
				{
					if(column_expand_status_[j]) {
						resize(p, average_expandable_width, p->size().height());
					} else {
						resize(p, column_width_list_[j], p->size().height());
					}
					move(p, xpos, p->position().y());

					xpos = xpos + p->size().width() + space_;

					j++;
					if(j == column_) {
						xpos = x;
						j = 0;
						i++;
					}
				}

			} else {

				for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
				{
					if(column_expand_status_[j]) {
						resize(p, 0, p->size().height());
					} else {
						resize(p, valid_width * column_width_list_[j] / total_fixed_column_width_, p->size().height());
					}
					move(p, xpos, p->position().y());

					xpos = xpos + p->size().width() + space_;

					j++;
					if(j == column_) {
						xpos = x;
						j = 0;
						i++;
					}
				}

			}

		} else {

			for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
			{
				if(column_expand_status_[j]) {
					resize(p, 0, p->size().height());
				} else {
					resize(p, valid_width * column_width_list_[j] / total_fixed_column_width_, p->size().height());
				}
				move(p, xpos, p->position().y());

				xpos = xpos + p->size().width() + space_;

				j++;
				if(j == column_) {
					xpos = x;
					j = 0;
					i++;
				}
			}

		}
	}

	void TableAdjustment::DistributeHorizontallyWithPreferredWidth (int x, int width) const
	{
		unsigned int i = 0;	// row
		unsigned int j = 0;	// column

		int xpos = x;
		for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
		{
			resize(p, column_width_list_[j], p->size().height());
			move(p, xpos, p->position().y());

			xpos = xpos + p->size().width() + space_;

			j++;
			if(j == column_) {
				xpos = x;
				j = 0;
				i++;
			}
		}
	}

	void TableAdjustment::DistributeHorizontallyWithLargeWidth (int x, int width) const
	{
		unsigned int i = 0;	// row
		unsigned int j = 0;	// column

		int expandable_column_num = column_ - fixed_column_num_;
		int average_expandable_column_width = 0;

		if (expandable_column_num > 0) {
			average_expandable_column_width = (width - (column_ - 1) * space_
			        - total_fixed_column_width_) / expandable_column_num;
		}

		int xpos = x;
		for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
		{
			if(column_expand_status_[j]) {
				resize(p, average_expandable_column_width, p->size().height());
			} else {
				resize(p, column_width_list_[j], p->size().height());
			}
			move(p, xpos, p->position().y());

			xpos = xpos + p->size().width() + space_;

			j++;
			if(j == column_) {
				xpos = x;
				j = 0;
				i++;
			}
		}
	}

	void TableAdjustment::DistributeVerticallyWithSmallHeight (int y, int height) const
	{
		int valid_height = height - (row_ - 1) * space_;

		unsigned int i = 0;	// row
		unsigned int j = 0;	// column

		int ypos = y + height;

		if(valid_height <= 0) {
			for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
			{
				resize(p, p->size().width(), 0);
				move(p, p->position().x(), ypos);

				j++;
				if(j == column_) {
					j = 0;
					i++;
				}
			}

			return;
		}

		if(expand_y_) {

			assert(fixed_row_num_ != (int)row_);

			int average_expandable_height = (valid_height - total_fixed_row_height_) / (row_ - fixed_row_num_);

			if(average_expandable_height > 0) {

				for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
				{
					if(row_expand_status_[i]) {
						resize(p, p->size().width(), average_expandable_height);
					} else {
						resize(p, p->size().width(), row_height_list_[i]);
					}
					move(p, p->position().x(), ypos - p->size().height());

					j++;
					if(j == column_) {
						ypos = ypos - p->size().height() - space_;
						j = 0;
						i++;
					}
				}

			} else {

				for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
				{
					if(row_expand_status_[i]) {
						resize(p, p->size().width(), 0);
					} else {
						resize(p, p->size().width(), valid_height * row_height_list_[i] / total_fixed_row_height_);
					}
					move(p, p->position().x(), ypos - p->size().height());

					j++;
					if(j == column_) {
						ypos = ypos - p->size().height() - space_;
						j = 0;
						i++;
					}
				}

			}

		} else {

			for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
			{
				if(row_expand_status_[i]) {
					resize(p, p->size().width(), 0);
				} else {
					resize(p, p->size().width(), valid_height * row_height_list_[i] / total_fixed_row_height_);
				}
				move(p, p->position().x(), ypos - p->size().height());

				j++;
				if(j == column_) {
					ypos = ypos - p->size().height() - space_;
					j = 0;
					i++;
				}
			}

		}

	}

	void TableAdjustment::DistributeVerticallyWithPreferredHeight (int y, int height) const
	{
		unsigned int i = 0;	// row
		unsigned int j = 0;	// column

		int ypos = y + height;
		for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
		{
			resize(p, p->size().width(), row_height_list_[i]);
			move(p, p->position().x(), ypos - p->size().height());

			j++;
			if(j == column_) {
				ypos = ypos - row_height_list_[i] - space_;
				j = 0;
				i++;
			}
		}
	}

	void TableAdjustment::DistributeVerticallyWithLargeHeight (int y, int height) const
	{
		unsigned int i = 0;	// row
		unsigned int j = 0;	// column

		int expandable_row_num = row_ - fixed_row_num_;
		int average_expandable_row_height = 0;

		if (expandable_row_num > 0) {
			average_expandable_row_height = (height - (row_ - 1) * space_
			        - total_fixed_row_height_) / expandable_row_num;
		}

		int ypos = y + height;
		for(AbstractView* p = view()->first_subview(); p; p = p->next_view())
		{
			if(row_expand_status_[i]) {
				resize(p, p->size().width(), average_expandable_row_height);
			} else {
				resize(p, p->size().width(), row_height_list_[i]);
			}
			move(p, p->position().x(), ypos - p->size().height());

			j++;
			if(j == column_) {
				ypos = ypos - p->size().height() - space_;
				j = 0;
				i++;
			}
		}
	}

}
