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
		set_preferred_size(400, 400);
		set_size(400, 400);
		set_expand_x(true);
		set_expand_y(true);
	}

	Splitter::~Splitter ()
	{

	}
	
	void Splitter::Add (AbstractWidget* widget)
	{
		unsigned int room = GetAverageRoom(m_orientation);

		if(m_orientation == Horizontal) {

			unsigned int height = size().height() - margin().top() - margin().bottom();

			if(AddSubWidget(widget)) {
				ResizeSubWidget(widget, room, height);
			}

		} else {

			unsigned int width = size().width() - margin().left() - margin().right();

			if(AddSubWidget(widget)) {
				ResizeSubWidget(widget, width, room);
			}

		}
	}

	void Splitter::Remove (AbstractWidget* widget)
	{
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
					break;
				}

				case FormPosition: {

					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					MoveSubWidgets(x, y);

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
	
	void Splitter::AlighSubWidgets ()
	{
	}

	unsigned int Splitter::GetAverageRoom (Orientation orientation)
	{
		unsigned int room = 0;

		if(orientation == Horizontal) {
			room = size().width() - margin().left() - margin().right();
			if(sub_widget_size()) {
				room = room - (m_space * (sub_widget_size() - 1));
				room = room / sub_widget_size();
			}
		} else {
			room = size().height() - margin().top() - margin().bottom();
			if(sub_widget_size()) {
				room = room - (m_space * (sub_widget_size() - 1));
				room = room / sub_widget_size();
			}
		}

		return room;
	}

}
