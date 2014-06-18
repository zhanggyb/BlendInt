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

#include <BlendInt/Gui/HBlock.hpp>

namespace BlendInt {

	HBlock::HBlock ()
	: AbstractDequeContainer()
	{
		set_size(100, 20);
		set_margin(0, 0, 0, 0);
	}
	
	HBlock::~HBlock ()
	{
	}
	
	void HBlock::PushBack (Widget* widget)
	{
		AbstractWidget* last = 0;
		if(sub_widget_size()) {
			last = sub_widgets()->back();
		}

		if(PushBackSubWidget(widget)) {

			widget->SetEmboss(true);
			FillSubWidgetsAveragely(position(), size(), margin(), Horizontal, AlignCenter, -1);

			if(last) {
				last->SetRoundCornerType(last->round_corner_type() & ~(RoundTopRight | RoundBottomRight));
				widget->SetRoundCornerType(RoundTopRight | RoundBottomRight);
			} else {
				widget->SetRoundCornerType(RoundAll);
			}

		}
	}
	
	bool HBlock::IsExpandX () const
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
	
	Size HBlock::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {

			preferred_size.set_width(100);
			preferred_size.set_height(20);

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
			preferred_size.set_width(sub_widget_size() * max_width);
			preferred_size.set_height(max_height);

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}
	
	void HBlock::UpdateContainer (const WidgetUpdateRequest& request)
	{
		if(request.target() == this) {

			switch(request.type()) {

				case ContainerMargin: {

					const Margin* margin_p = static_cast<const Margin*>(request.data());
					FillSubWidgetsAveragely(position(), size(), *margin_p, Horizontal, AlignCenter, -1);

					break;
				}

				case ContainerRefresh: {
					Refresh();
					break;
				}

			}
		}
	}
	
	void HBlock::UpdateGeometry (const WidgetUpdateRequest& request)
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
					FillSubWidgetsAveragely(position(), *size_p, margin(), Horizontal, AlignCenter, -1);

					break;
				}

				default: {
					break;
				}
			}

		}
	}
	
	ResponseType HBlock::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	ResponseType HBlock::CursorEnterEvent (bool entered)
	{
		return IgnoreAndContinue;
	}
	
	ResponseType HBlock::KeyPressEvent (const KeyEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	ResponseType HBlock::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	ResponseType HBlock::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	ResponseType HBlock::MousePressEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	ResponseType HBlock::MouseReleaseEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	ResponseType HBlock::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

}
