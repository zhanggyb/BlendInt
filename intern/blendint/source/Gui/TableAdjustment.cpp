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

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Gui/TableAdjustment.hpp>

namespace BlendInt {

	TableAdjustment::TableAdjustment(AbstractView* view, unsigned int row, unsigned int column, int space)
	: AbstractAdjustment(view),
	  row_(row),
	  column_(column),
	  space_(space),
	  expand_x_(false),
	  expand_y_(false)
	{
		assert(view->subs_count() == (int)(row * column));

		column_expand_status_.resize(column_, false);
		row_expand_status_.resize(row_, false);

		column_width_list_.resize(column_, 0);
		row_height_list_.resize(row_, 0);
	}

	TableAdjustment::~TableAdjustment ()
	{
	}

	void TableAdjustment::Adjust (int x, int y, int w, int h)
	{
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
			DistributeVerticallyWithSmallHeight();
		} else if (h > total_preferred_size.height()) {
			DistributeVerticallyWithLargeHeight();
		} else {
			DistributeVerticallyWithPreferredHeight();
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
	}

	Size TableAdjustment::GetTotalPreferredSize() const
	{

		int total_width = (column_ - 1) * space_;
		int total_height = (row_ - 1) * space_;

		for(size_t i = 0; i < column_width_list_.size(); i++)
		{
			total_width += column_width_list_[i];
		}

		for(size_t i = 0; i < row_height_list_.size(); i++)
		{
			total_height += row_height_list_[i];
		}

		return Size(total_width, total_height);
	}

	void TableAdjustment::DistributeHorizontallyWithSmallWidth (int x, int width) const
	{
		DBG_PRINT_MSG("%s", "small width");
	}

	void TableAdjustment::DistributeHorizontallyWithPreferredWidth (int x, int width) const
	{
		DBG_PRINT_MSG("%s", "preferred width");
	}

	void TableAdjustment::DistributeHorizontallyWithLargeWidth (int x, int width) const
	{
		DBG_PRINT_MSG("%s", "large width");

		unsigned int i = 0;	// row
		unsigned int j = 0;	// column

		if(expand_x_) {

		} else {

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

	}

	void TableAdjustment::DistributeVerticallyWithSmallHeight () const
	{
		DBG_PRINT_MSG("%s", "small height");
	}

	void TableAdjustment::DistributeVerticallyWithPreferredHeight () const
	{
		DBG_PRINT_MSG("%s", "preferred height");
	}

	void TableAdjustment::DistributeVerticallyWithLargeHeight () const
	{
		DBG_PRINT_MSG("%s", "large height");
	}

}
