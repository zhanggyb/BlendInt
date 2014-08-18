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

#include <BlendInt/Gui/VBlockLayout.hpp>

namespace BlendInt {

	VBlockLayout::VBlockLayout ()
	: AbstractContainer()
	{
		set_size(80, 60);
		set_margin(0, 0, 0, 0);
	}

	VBlockLayout::~VBlockLayout ()
	{
	}

	void VBlockLayout::PushFront (AbstractWidget* widget)
	{
		AbstractWidget* orig_first = first();

		if(PushBackSubWidget(widget)) {

			if(orig_first) {
				widget->SetEmboss(false);
				SetSubWidgetRoundType(orig_first, orig_first->round_type() & ~(RoundBottomLeft | RoundBottomRight));
				SetSubWidgetRoundType(widget, RoundBottomLeft | RoundBottomRight);
			} else {
				widget->SetEmboss(true);
				SetSubWidgetRoundType(widget, RoundAll);
			}

			FillInVBlock(position(), size(), margin());

		}
	}

	void VBlockLayout::PushBack (AbstractWidget* widget)
	{
		AbstractWidget* orig_last = last();

		if(PushBackSubWidget(widget)) {

			widget->SetEmboss(true);

			if(orig_last) {
				orig_last->SetEmboss(false);
				SetSubWidgetRoundType(orig_last, orig_last->round_type() & ~(RoundBottomLeft | RoundBottomRight));
				SetSubWidgetRoundType(widget, RoundBottomLeft | RoundBottomRight);
			} else {
				SetSubWidgetRoundType(widget, RoundAll);
			}

			FillInVBlock(position(), size(), margin());

		}
	}

	bool VBlockLayout::IsExpandX() const
	{
		bool expand = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool VBlockLayout::IsExpandY () const
	{
		bool expand = false;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size VBlockLayout::GetPreferredSize () const
	{
		Size preferred_size;

		if(first() == 0) {

			preferred_size.set_width(80);
			preferred_size.set_height(60);

		} else {

			Size tmp;
			int max_width = 0;
			int max_height = 0;
			int sum = 0;

			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if(p->visiable()) {
					sum++;
					tmp = p->GetPreferredSize();

					max_width = std::max(max_width, tmp.width());
					max_height = std::max(max_height, tmp.height());
				}
			}
			preferred_size.set_width(max_width);
			preferred_size.set_height(sum * (max_height - 1));

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	void VBlockLayout::PerformMarginUpdate(const Margin& request)
	{
		FillInVBlock(position(), size(), request);
	}

	bool VBlockLayout::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		// Do not allow sub widget changing its size
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	bool VBlockLayout::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		// Do not allow sub widget changing its position
		if(request.source()->container() == this) {
			return false;
		}

		return true;
	}

	void VBlockLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			FillInVBlock(position(), *request.size(), margin());
		}

		ReportSizeUpdate(request);
	}

	void VBlockLayout::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		ReportPositionUpdate(request);
	}

	ResponseType VBlockLayout::Draw (const Profile& profile)
	{
		return Ignore;
	}

	ResponseType VBlockLayout::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType VBlockLayout::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType VBlockLayout::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType VBlockLayout::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType VBlockLayout::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType VBlockLayout::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType VBlockLayout::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}
	
	void VBlockLayout::FillInVBlock (const Point& out_pos, const Size& out_size,
					const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		FillInVBlock(x, y, w, h);
	}

	void VBlockLayout::FillInVBlock (int x, int y, int w, int h)
	{
		int count = widget_count();
		if(count == 0) return;
		int average_height = h / count + 1;

		y = y + h;
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			ResizeSubWidget(p, w, average_height);
			y = y - average_height + 1;
			SetSubWidgetPosition(p, x, y);
		}
	}

}
