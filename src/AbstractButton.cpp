/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/AbstractButton.hpp>

namespace BIL {

	AbstractButton::AbstractButton (Widget *parent)
		: Widget(parent), down_(false), checkable_(false),
		  checked_(false), hover_(false)
	{
	}

	AbstractButton::~AbstractButton ()
	{
		// TODO Auto-generated destructor stub
	}

	void AbstractButton::set_text (const wstring& text)
	{
		if(text.empty()) {
			// TODO: draw blank label
			return;
		}

		text_.Append(text);

		calculateBox();

		text_.set_origin(Coord3f(
								 pos_.x() + padding_.left(),
								 pos_.y() + padding_.bottom(),
								 0.0)
						 );
	}

	void AbstractButton::calculateBox(void)
	{
		Size box = text_.CalculateOutlineBoxSize();

		box.set_width(box.width() + padding_.left() + padding_.right());
		box.set_height(box.height() + padding_.top() + padding_.bottom());

		resize (box.width(), box.height());
	}

	void AbstractButton::MousePressEvent (MouseEvent* event)
	{
		if (!size_.Contains(event->pos()))
		{
			event->Ignore();
			return;
		}

		if (checkable_) {
			checked_ = !checked_;
			// TODO: emit check event
		} else {
			down_ = true;
			clicked_.fire();
		}
	}

	void AbstractButton::MouseReleaseEvent(MouseEvent* event)
	{
		down_ = false;
		
		if (! size_.Contains(event->pos()))
		{
			event->Ignore();
			return;
		}

		if (checkable_) {
			// TODO: add code for checkable
		} else {

		}
	}

	void AbstractButton::MouseMoveEvent (MouseEvent* event)
	{
		if (size_.Contains(event->pos())) {
			hover_ = true;
            event->Accept();
		} else {
			hover_ = false;
			down_ = false;
		}
	}

} /* namespace BIL */
