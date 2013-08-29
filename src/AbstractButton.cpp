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

		text_.append(text);

		calculateBox();

		text_.setOrigin(Coord3f(
								 pos_.x() + padding_.left(),
								 pos_.y() + padding_.bottom(),
								 0.0)
						 );
	}

	void AbstractButton::calculateBox(void)
	{
		Size box = text_.calculateOutlineBoxSize();

		box.set_width(box.width() + padding_.left() + padding_.right());
		box.set_height(box.height() + padding_.top() + padding_.bottom());

		resize (box.width(), box.height());
	}

	void AbstractButton::mousePressEvent (MouseEvent* event)
	{
		if (!size_.Contains(event->pos()))
		{
			event->ignore();
			return;
		}

		if (checkable_) {
			checked_ = !checked_;
			toggled_.fire(checked_);

		} else {
			down_ = true;
			clicked_.fire();
		}
		event->accept();
	}

	void AbstractButton::mouseReleaseEvent(MouseEvent* event)
	{
		down_ = false;
		
		if (! size_.Contains(event->pos()))
		{
			event->ignore();
			return;
		}

		if (checkable_) {

		} else {

		}
	}

	void AbstractButton::mouseMoveEvent (MouseEvent* event)
	{
		if (size_.Contains(event->pos())) {
			hover_ = true;
            event->accept();
		} else {
			hover_ = false;
			down_ = false;
		}
	}

} /* namespace BIL */
