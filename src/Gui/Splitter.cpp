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

#include <BlendInt/Gui/Splitter.hpp>

namespace BlendInt {

	Splitter::Splitter(Orientation orientation)
	: AbstractDequeContainer(),
	  m_orientation(orientation),
	  m_space(2)
	{
		set_size(400, 400);
		set_expand_x(true);
		set_expand_y(true);
	}

	Splitter::~Splitter ()
	{

	}
	
	void Splitter::Add (AbstractWidget* widget)
	{
		if(AddSubWidget(widget)) {
			AlignSubWidgets(m_orientation, size(), margin(), m_space);
		}
	}

	void Splitter::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {
			AlignSubWidgets(m_orientation, size(), margin(), m_space);
		}
	}

	bool Splitter::UpdateTest (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case SubWidgetSize:
					return false;	// DO not allow sub widget geometry reset outside

				case SubWidgetPosition:
					return false;

				default:
					return AbstractDequeContainer::UpdateTest(request);

			}

		} else {
			return false;
		}
	}
	
	void Splitter::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch(request.type()) {

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());
					AlignSubWidgets(m_orientation,*size_p, margin(), m_space);

					break;
				}

				case FormPosition: {

					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					MoveSubWidgets(x, y);

					break;
				}

				case WidgetRefresh: {
					Refresh();
					break;
				}

				default:
					break;

			}

		}
	}

	ResponseType Splitter::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	void Splitter::AlignSubWidgets (Orientation orienation, const Size& size, const Margin& margin, int space)
	{
		unsigned int room = GetAverageRoom(orienation, size, margin, space);
		int x = position().x() + margin.left();

		if(orienation == Horizontal) {

			int y = position().y() + margin.bottom();
			unsigned int h = size.height() - margin.top() - margin.bottom();

			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				ResizeSubWidget(*it, room, h);
				SetSubWidgetPosition(*it, x, y);
				x = x + room + space;
			}

		} else {

			int y = position().y() + size.height() - margin.top();
			unsigned int w = size.width() - margin.left() - margin.right();

			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				ResizeSubWidget(*it, w, room);
				SetSubWidgetPosition(*it, x, y);
				y = y - room - space;
			}

		}
	}

	ResponseType Splitter::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Splitter::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	unsigned int Splitter::GetAverageRoom (Orientation orientation, const Size& size, const Margin& margin, int space)
	{
		unsigned int room = 0;

		if(orientation == Horizontal) {
			room = size.width() - margin.left() - margin.right();
			if(sub_widget_size()) {
				room = room - (space * (sub_widget_size() - 1));
				room = room / sub_widget_size();
			}
		} else {
			room = size.height() - margin.top() - margin.bottom();
			if(sub_widget_size()) {
				room = room - (space * (sub_widget_size() - 1));
				room = room / sub_widget_size();
			}
		}

		return room;
	}

}
