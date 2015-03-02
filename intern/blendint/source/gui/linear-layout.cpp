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

#include <gui/linear-layout.hpp>
#include <gui/linear-adjustment.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	LinearLayout::LinearLayout (Orientation orient, int align, int space)
	: AbstractLayout(),
	  orientation_(orient),
	  alignment_(align),
	  space_(space)
	{
		set_size (200, 200);
	}

	LinearLayout::LinearLayout (int width, int height, const Margin& margin, Orientation orient, int align, int space)
	: AbstractLayout(width, height, margin),
	  orientation_(orient),
	  alignment_(align),
	  space_(space)
	{
	}

	LinearLayout::~LinearLayout ()
	{
	}

	bool LinearLayout::AddWidget(AbstractWidget* widget)
	{
		if(PushBackSubView(widget)) {
			Adjust();
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool LinearLayout::InsertWidget(int index, AbstractWidget* widget)
	{
		if(InsertSubView(index, widget)) {
			Adjust();
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool LinearLayout::InsertWidget(int row, int column,
			AbstractWidget* widget)
	{
		if(orientation_ == Horizontal) {

			if(row != 0) {
				DBG_PRINT_MSG("Error: %s", "LinearLayout contains only 1 row, and the 1st parameter will be ignored");
			}

			if(InsertSubView(column, widget)) {
				Adjust();
				RequestRedraw();
				return true;
			}

		} else {

			if(column != 0) {
				DBG_PRINT_MSG("Error: %s", "LinearLayout contains only 1 column, and the 2nd parameter will be ignored");
			}

			if(InsertSubView(row, widget)) {
				Adjust();
				RequestRedraw();
				return true;
			}

		}

		return false;
	}

	void LinearLayout::Adjust ()
	{
		int x = margin().left();
		int y = margin().bottom();
		int width = size().width() - margin().hsum();
		int height = size().height() - margin().vsum();

		LinearAdjustment adjustment(this, orientation_, alignment_, space_);
		adjustment.Adjust(x, y, width, height);
	}

	bool LinearLayout::Remove (AbstractWidget* widget)
	{
		if(RemoveSubView(widget)) {
			Adjust();
			return true;
		}

		return false;
	}

	void LinearLayout::SetOrientation (Orientation orient)
	{
		if(orientation_ == orient)
			return;

		orientation_ = orient;
		Adjust();
		RequestRedraw();
	}

	void LinearLayout::SetAlignment (int align)
	{
		if(alignment_ == align) return;

		alignment_ = align;
		Adjust();
		RequestRedraw();
	}

	void LinearLayout::SetSpace (int space)
	{
		if(space_ == space) return;

		space_ = space;
		Adjust();
		RequestRedraw();
	}

	Size BlendInt::LinearLayout::GetPreferredSize () const
	{
		if(subs_count() == 0) {
			return Size(200, 200);
		}

		int w = 0;
		int h = 0;

		Size tmp;

		if(orientation_ == Horizontal) {
			w = -space_;
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(p->visiable()) {
					tmp = p->GetPreferredSize();

					w += (tmp.width() + space_);
					h = std::max(h, tmp.height());
				}
			}

			w += pixel_size(margin().hsum());
			h += pixel_size(margin().vsum());
		} else {
			h = -space_;
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(p->visiable()) {
					tmp = p->GetPreferredSize();

					w = std::max(w, tmp.width());
					h += (tmp.height() + space_);
				}
			}

			w += pixel_size(margin().hsum());
			h += pixel_size(margin().vsum());
		}

		return Size(w, h);
	}

	bool LinearLayout::IsExpandX () const
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

	bool LinearLayout::IsExpandY () const
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

	void LinearLayout::PerformMarginUpdate(const Margin& request)
	{
		set_margin(request);

		if(subs_count()) {
			Adjust();
			RequestRedraw();
		}
	}

	bool LinearLayout::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		// Do not allow sub widget changing its size
		if(request.source()->superview() == this) {
			return false;
		}

		return true;
	}

	bool LinearLayout::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		// Do not allow sub widget changing its position
		if(request.source()->superview() == this) {
			return false;
		}

		return true;
	}

	void LinearLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			Adjust();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

}
