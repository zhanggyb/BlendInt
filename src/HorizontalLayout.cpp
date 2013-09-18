/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <algorithm>
#include <BILO/HorizontalLayout.hpp>

#include <BILO/Interface.hpp>

namespace BILO {

	HorizontalLayout::HorizontalLayout(Drawable* parent, int align)
		: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	HorizontalLayout::~HorizontalLayout()
	{

	}

	void HorizontalLayout::update (int property)
	{
		//if (property == WidgetPropertySize) {
		unsigned int total_width = 0;
		unsigned int total_height = 0;
		unsigned int max_widget_height = 0;

		std::list<Traceable*>::const_iterator it;
		total_width = padding_.left();
		for (it = m_children.begin(); it != m_children.end(); it++)
		{
			Drawable* child = dynamic_cast<Drawable*>(*it);
			if(child) {
				child->set_pos(pos_.x() + child->margin().left() + total_width, pos_.y() + child->margin().bottom() + padding_.bottom());
				total_width = total_width + child->margin().left() + child->size().width() + child->margin().right();
				total_height = std::max (total_height, padding_.top() + child->margin().top() + child->size().height() + child->margin().bottom() + padding_.bottom());
				max_widget_height = std::max (max_widget_height, child->size().height());
			}
		}
		total_width += padding_.right();

		for (it = m_children.begin(); it != m_children.end(); it++)
		{
			Drawable* child = dynamic_cast<Drawable*>(*it);
			if(child) {
				if(alignment_ & AlignTop) {
					child->set_pos(child->pos().x(),
								   pos_.y() +
								   (total_height -
									(padding_.top() + child->margin().top() + child->size().height())));
				} else if (alignment_ & AlignBottom) {
					child->set_pos(child->pos().x(), pos_.y() + child->margin().bottom() + padding_.bottom());
				} else if (alignment_ & AlignHorizontalCenter) {
					child->set_pos(child->pos().x(),
								   pos_.y() + padding_.bottom() +
								   (max_widget_height - child->size().height()) / 2 + child->margin().bottom());
				}
			}
		}

		size_.set_width(total_width);
		size_.set_height(total_height);
		//}
	}

	void HorizontalLayout::render ()
	{
		list<Traceable*>::const_iterator it;
		Drawable *item = 0;
		for (it = children().begin(); it != children().end(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item) {
				Interface::instance()->dispatch_render_event(m_z, item);
			}
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());
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

		glPopMatrix();
#endif
	}

}
