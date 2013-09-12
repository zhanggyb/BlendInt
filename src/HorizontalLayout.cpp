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
				child->set_pos(pos_.x() + child->margin().left() + total_width, pos_.y() + child->margin().bottom());
				total_width = total_width + child->margin().left() + child->size().width() + child->margin().right();
				total_height = std::max (total_height, child->margin().top() + child->size().height() + child->margin().bottom());
			}
		}

		size_.set_width(total_width);
		size_.set_height(total_height);
	}

	void HorizontalLayout::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());
#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(size_.width(), 0);
			glVertex2i(size_.width(), size_.height());
			glVertex2i(0, size_.height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif

		glPopMatrix();
	}

}
