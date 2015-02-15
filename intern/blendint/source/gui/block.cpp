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

#include <gui/block.hpp>

namespace BlendInt {

	Block::Block(Orientation orienatiaon)
	: AbstractRoundWidget (),
	  orientation_(orienatiaon)
	{
		if(orienatiaon == Horizontal) {
			set_size(100, 20);
		} else {
			set_size(80, 60);
		}
	}

	Block::~Block()
	{
	}

	void Block::AddWidget(AbstractRoundWidget* widget)
	{
		AbstractRoundWidget* orig_last = dynamic_cast<AbstractRoundWidget*>(last_subview());

		if(PushBackSubView(widget)) {

			widget->SetEmboss(true);

			if(orientation_ == Horizontal) {

				if(orig_last) {
					orig_last->SetRoundType(orig_last->round_type() &
											~(RoundTopRight | RoundBottomRight));
					widget->SetRoundType(RoundTopRight | RoundBottomRight);
				} else {
					widget->SetRoundType(RoundAll);
				}

				FillInHBlock(size());

			} else {

				if(orig_last) {
					orig_last->SetEmboss(false);
					orig_last->SetRoundType(orig_last->round_type() &
											~(RoundBottomLeft | RoundBottomRight));
					widget->SetRoundType(RoundBottomLeft | RoundBottomRight);
				} else {
					widget->SetRoundType(RoundAll);
				}

				FillInVBlock(size());
			}

		}
	}

	void Block::InsertWidget(int index, AbstractRoundWidget* widget)
	{
		// TODO: implement
	}

	bool Block::IsExpandX() const
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

	bool Block::IsExpandY() const
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

	Size Block::GetPreferredSize() const
	{
		Size preferred_size;

		if(subs_count() == 0) {

			if(orientation_ == Horizontal) {
				preferred_size.reset(100, 20);
			} else {
				preferred_size.reset(80, 60);
			}

		} else {

			Size tmp;
			int max_width = 0;
			int max_height = 0;
			int sum = 0;

			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(p->visiable()) {
					sum++;
					tmp = p->GetPreferredSize();

					max_width = std::max(max_width, tmp.width());
					max_height = std::max(max_height, tmp.height());
				}
			}

			if(orientation_ == Horizontal) {
				preferred_size.set_width(sum * (max_width - 1));
				preferred_size.set_height(max_height);
			} else {
				preferred_size.set_width(max_width);
				preferred_size.set_height(sum * (max_height - 1));
			}

		}

		return preferred_size;
	}

	void Block::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			if(orientation_ == Horizontal) {
				FillInHBlock(*request.size());
			} else {
				FillInVBlock(*request.size());
			}

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Block::FillInHBlock(const Size& out_size)
	{
		int x = 0;
		int y = 0;
		int w = out_size.width();
		int h = out_size.height();

		FillInHBlock(x, y, w, h);
	}

	void Block::FillInHBlock(int x, int y, int w, int h)
	{
		int count = subs_count();
		if(count == 0) return;
		int average_width = w / count + 1;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			ResizeSubView(p, average_width, h);
			MoveSubViewTo(p, x, y);
			x = x + average_width - 1;
		}
	}

	void Block::FillInVBlock(const Size& out_size)
	{
		int x = 0;
		int y = 0;
		int w = out_size.width();
		int h = out_size.height();

		FillInVBlock(x, y, w, h);
	}

	Response Block::Draw (AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	void Block::FillInVBlock(int x, int y, int w, int h)
	{
		int count = subs_count();
		if(count == 0) return;
		int average_height = h / count + 1;

		y = y + h;
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			ResizeSubView(p, w, average_height);
			y = y - average_height + 1;
			MoveSubViewTo(p, x, y);
		}
	}

}
