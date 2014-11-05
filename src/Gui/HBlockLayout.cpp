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

#include <BlendInt/Gui/HBlockLayout.hpp>

namespace BlendInt {

	HBlockLayout::HBlockLayout ()
	: Layout()
	{
		set_size(100, 20);
		set_margin(0, 0, 0, 0);
	}
	
	HBlockLayout::~HBlockLayout ()
	{
	}
	
	void HBlockLayout::Prepend (Widget* widget)
	{
		AbstractWidget* orig_first = first_child();

		if(PushFrontSubWidget(widget)) {

			widget->SetEmboss(true);

			if(orig_first) {
				SetSubWidgetRoundType(orig_first, orig_first->round_type() & ~(RoundTopLeft | RoundBottomLeft));
				SetSubWidgetRoundType(widget, RoundTopLeft | RoundBottomLeft);
			} else {
				SetSubWidgetRoundType(widget, RoundAll);
			}

			FillInHBlock(size(), margin());

		}
	}

	void HBlockLayout::Append (Widget* widget)
	{
		AbstractWidget* orig_last = last_child();

		if(PushBackSubWidget(widget)) {

			widget->SetEmboss(true);

			if(orig_last) {
				SetSubWidgetRoundType(orig_last, orig_last->round_type() & ~(RoundTopRight | RoundBottomRight));
				SetSubWidgetRoundType(widget, RoundTopRight | RoundBottomRight);
			} else {
				SetSubWidgetRoundType(widget, RoundAll);
			}

			FillInHBlock(size(), margin());

		}
	}
	
	bool HBlockLayout::IsExpandX () const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool HBlockLayout::IsExpandY () const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}
	
	Size HBlockLayout::GetPreferredSize () const
	{
		Size preferred_size;

		if(first_child() == 0) {

			preferred_size.set_width(100);
			preferred_size.set_height(20);

		} else {

			Size tmp;
			int max_width = 0;
			int max_height = 0;
			int sum = 0;

			for(AbstractWidget* p = first_child(); p; p = p->next())
			{
				if(p->visiable()) {
					tmp = p->GetPreferredSize();

					sum++;
					max_width = std::max(max_width, tmp.width());
					max_height = std::max(max_height, tmp.height());
				}
			}
			preferred_size.set_width(sum * (max_width - 1));
			preferred_size.set_height(max_height);

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}
	
	void HBlockLayout::PerformMarginUpdate(const Margin& request)
	{
		FillInHBlock(size(), request);
	}

	bool HBlockLayout::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		// Do not allow sub widget changing its size
		if(request.source()->parent() == this) {
			return false;
		}

		return true;
	}

	bool HBlockLayout::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		// Do not allow sub widget changing its position
		if(request.source()->parent() == this) {
			return false;
		}

		return true;
	}

	void HBlockLayout::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			FillInHBlock(*request.size(), margin());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}
	
	void HBlockLayout::FillInHBlock (const Size& out_size,
					const Margin& margin)
	{
		int x = margin.left();
		int y = margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		FillInHBlock(x, y, w, h);
	}

	void HBlockLayout::FillInHBlock (int x, int y, int w, int h)
	{
		int count = subs_count();
		if(count == 0) return;
		int average_width = w / count + 1;

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			ResizeSubWidget(p, average_width, h);
			SetSubWidgetPosition(p, x, y);
			x = x + average_width - 1;
		}
	}

}
