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

#include <BlendInt/Gui/VBlock.hpp>

namespace BlendInt {

	VBlock::VBlock ()
	: AbstractDequeContainer()
	{
		set_size(80, 60);
		set_margin(0, 0, 0, 0);
	}

	VBlock::~VBlock ()
	{
	}

	void VBlock::PushBack (AbstractWidget* widget)
	{
		AbstractWidget* last = 0;
		if(sub_widget_size()) {
			last = sub_widgets()->back();
		}

		if(PushBackSubWidget(widget)) {

			widget->SetEmboss(true);
			FillInVBlock(position(), size(), margin());

			if(last) {
				last->SetEmboss(false);
				last->SetRoundCornerType(last->round_corner_type() & ~(RoundBottomLeft | RoundBottomRight));
				widget->SetRoundCornerType(RoundBottomLeft | RoundBottomRight);
			} else {
				widget->SetRoundCornerType(RoundAll);
			}

		}
	}

	bool VBlock::IsExpandX() const
	{
		bool expand = false;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool VBlock::IsExpandY () const
	{
		bool expand = false;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			if((*it)->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size VBlock::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {

			preferred_size.set_width(80);
			preferred_size.set_height(60);

		} else {

			AbstractWidget* widget = 0;
			Size tmp;
			int max_width = 0;
			int max_height = 0;

			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				widget = *it;

				if(widget->visiable()) {
					tmp = widget->GetPreferredSize();

					max_width = std::max(max_width, tmp.width());
					max_height = std::max(max_height, tmp.height());
				}
			}
			preferred_size.set_width(max_width);
			preferred_size.set_height(sub_widget_size() * (max_height - 1));

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	void VBlock::UpdateContainer (const WidgetUpdateRequest& request)
	{
		if(request.target() == this) {

			switch(request.type()) {

				case ContainerMargin: {

					const Margin* margin_p = static_cast<const Margin*>(request.data());
					FillInVBlock(position(), size(), *margin_p);

					break;
				}

				default: {
					ReportUpdateRequest(request);
					break;
				}

			}
		}
	}

	void VBlock::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(request.target() == this) {

			switch(request.type()) {

				case WidgetPosition: {
					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();
					MoveSubWidgets(x, y);

					break;
				}

				case WidgetSize: {

					const Size* size_p = static_cast<const Size*>(request.data());
					FillInVBlock(position(), *size_p, margin());

					break;
				}

				default: {
					break;
				}
			}

		}
	}

	ResponseType VBlock::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBlock::CursorEnterEvent (bool entered)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBlock::KeyPressEvent (const KeyEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBlock::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBlock::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBlock::MousePressEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBlock::MouseReleaseEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType VBlock::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	void VBlock::FillInVBlock (const Point& out_pos, const Size& out_size,
					const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = out_size.width() - margin.hsum();
		int h = out_size.height() - margin.vsum();

		FillInVBlock(x, y, w, h);
	}
	
	void VBlock::FillInVBlock (int x, int y, int w, int h)
	{
		if(sub_widget_size() == 0) return;
		int average_height = h / sub_widget_size() + 1;

		AbstractWidget* widget = 0;

		y = y + h;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			widget = *it;

			ResizeSubWidget(widget, w, average_height);
			y = y - average_height + 1;
			SetSubWidgetPosition(widget, x, y);
		}
	}

}
