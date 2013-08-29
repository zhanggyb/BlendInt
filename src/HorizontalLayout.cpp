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

#include <algorithm>
#include <BIL/HorizontalLayout.hpp>

namespace BIL {

	HorizontalLayout::HorizontalLayout(Drawable* parent)
		: AbstractLayout(parent)
	{

	}

	HorizontalLayout::~HorizontalLayout()
	{

	}

	void HorizontalLayout::update ()
	{
		unsigned int total_width = 0;
		unsigned int total_height = 0;

		std::list<Traceable*>::const_iterator it;
		for (it = children_.begin(); it != children_.end(); it++)
		{
			Drawable* child = dynamic_cast<Drawable*>(*it);
			if(child) {
				// TODO: define alignment
				child->set_pos(pos_.x() + child->margin().left(), pos_.y() + child->margin().right());
				total_width = total_width + child->margin().left() + child->size().width() + child->margin().right();
				total_height = std::max (total_height, child->margin().top() + child->size().height() + child->margin().bottom());
			}
		}

		size_.set_width(total_width);
		size_.set_height(total_height);
	}

	void HorizontalLayout::render ()
	{
		std::list<Traceable*>::const_iterator it;
		for (it = children_.begin(); it != children_.end(); it++)
		{
			Drawable* child = dynamic_cast<Drawable*>(*it);
			if(child) {
				//child->render();
			}
		}
	}

}
