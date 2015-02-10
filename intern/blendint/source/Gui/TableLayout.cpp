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
#include <BlendInt/Gui/TableLayout.hpp>
#include <BlendInt/Gui/TableAdjustment.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	Cell::Cell()
	: AbstractWidget()
	{
		set_size(20, 20);
	}

	Cell::~Cell ()
	{

	}

	void Cell::SetWidget(AbstractWidget* widget)
	{
		if(subs_count()) {
			ClearSubViews();
			RequestRedraw();
		}

		if(PushBackSubView(widget)) {
			ResizeSubView(widget, size());
		}
	}

	bool Cell::IsExpandX () const
	{
		return subs_count() ? first_subview()->IsExpandX() : false;
	}

	bool Cell::IsExpandY () const
	{
		return subs_count() ? first_subview()->IsExpandY() : false;
	}

	Size Cell::GetPreferredSize () const
	{
		Size preferred_size(20, 20);

		if(subs_count()) {
			preferred_size = first_subview()->GetPreferredSize();
		}

		return preferred_size;
	}

	void Cell::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			if(subs_count()) {
				ResizeSubView(first_subview(), size());
				RequestRedraw();
			}

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	Response Cell::Draw (AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	// --------------------------------------

	TableLayout::TableLayout(unsigned int row, unsigned int column)
	: AbstractLayout(),
	  row_(row),
	  column_(column)
	{
		int width = 0;
		int height = 0;

		int row_width = 0;
		int row_height = 0;

		int x = pixel_size(margin().left());
		int y = pixel_size(margin().bottom());

		Cell* cell = 0;
		for(unsigned int i = 0; i < row_; i++) {

			row_width = 0;
			row_height = 0;

			for(unsigned int j = 0; j < column_; j++) {

				cell = Manage(new Cell);
				PushBackSubView(cell);

				MoveSubViewTo(cell, j * cell->size().width() + x, (row_ - i - 1) * cell->size().height() + y);

				row_width += cell->size().width();
				height = std::max(height, cell->size().height());

			}

			width = std::max(width, row_width);
			height += row_height;

		}

		width += pixel_size(margin().hsum());
		height += pixel_size(margin().vsum());

		set_size(width, height);
	}

	bool TableLayout::AddWidget (AbstractWidget* widget)
	{
		if(PushBackSubView(widget)) {

			return true;
		}

		return false;
	}

	bool TableLayout::InsertWidget (int index, AbstractWidget* widget)
	{
		if(InsertSubView(index, widget)) {

			return true;
		}

		return false;
	}

	bool TableLayout::InsertWidget (int row, int column,
	        AbstractWidget* widget)
	{
		int index = column_ * row + column;

		Cell* cell = dynamic_cast<Cell*>(GetSubViewAt(index));

		assert(cell);
		cell->SetWidget(widget);

		UpdateLayout();

		return true;
	}

	bool TableLayout::IsExpandX () const
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandX())
				return true;
		}

		return false;
	}

	bool TableLayout::IsExpandY () const
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandY())
				return true;
		}

		return false;
	}

	void TableLayout::UpdateLayout ()
	{

	}

	void TableLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			int x = pixel_size(margin().left());
			int y = pixel_size(margin().bottom());
			int w = size().width() - pixel_size(margin().hsum());
			int h = size().width() - pixel_size(margin().vsum());

			TableAdjustment adjust(this, row_, column_, 0);
			adjust.Adjust(x, y, w, h);

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void TableLayout::PerformMarginUpdate (const Margin& margin)
	{
		set_margin(margin);

		int x = pixel_size(this->margin().left());
		int y = pixel_size(this->margin().bottom());
		int w = size().width() - pixel_size(this->margin().hsum());
		int h = size().width() - pixel_size(this->margin().vsum());

		TableAdjustment adjust(this, row_, column_, 0);
		adjust.Adjust(x, y, w, h);

	}

	Size TableLayout::GetPreferredSize () const
	{
		int width = 0;
		int height = 0;

		int row_width = 0;
		int row_height = 0;

		unsigned int i = 0;	// row
		unsigned int j = 0;	// column
		Size tmp;

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {

			tmp = p->GetPreferredSize();
			row_width += tmp.width();
			row_height = std::max(row_height, tmp.height());

			j++;
			if(j == column_) {

				width = std::max(width, row_width);
				height += row_height;

				j = 0;
				i++;
			}
		}

		width += pixel_size(margin().hsum());
		height += pixel_size(margin().vsum());

		return Size(width, height);
	}

}
