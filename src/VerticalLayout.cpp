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
#include <BILO/VerticalLayout.hpp>

#include <BILO/Interface.hpp>

namespace BILO {

	VerticalLayout::VerticalLayout (int align)
			: AbstractLayout()
	{
		set_alignment(align);
	}

	VerticalLayout::VerticalLayout (int align, Drawable* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	VerticalLayout::~VerticalLayout ()
	{

	}

	void VerticalLayout::add_widget (Widget* widget)
	{
		m_list.push_back(widget);
		bind (widget);

		update(WidgetPropertySize);
	}

	void VerticalLayout::add_layout(AbstractLayout* layout)
	{
		m_list.push_back(layout);
		bind (layout);

		update(WidgetPropertySize);
	}

	bool VerticalLayout::remove (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);

		update(WidgetPropertySize);

		return true;
	}

	bool VerticalLayout::erase (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);

		delete object;

		update(WidgetPropertySize);

		return true;
	}

	void VerticalLayout::update (int property)
	{
		//if (property == WidgetPropertySize) {

			unsigned int total_width = 0;
			unsigned int total_height = 0;
			unsigned int max_widget_width = 0;

			std::list<Drawable*>::const_reverse_iterator rit;
			Drawable* child = 0;
			total_height = padding_.bottom();
			for (rit = m_list.rbegin(); rit != m_list.rend(); rit++) {
				child = *rit;
				if (child) {
					child->set_pos(
					        pos_.x() + child->margin().left() + padding_.left(),
					        pos_.y() + child->margin().bottom() + total_height);
					total_width = std::max(total_width,
					        padding_.left() + child->margin().left()
					                + child->size().width()
					                + child->margin().right()
					                + padding_.right());
					max_widget_width = std::max(max_widget_width,
					        child->size().width());
					total_height = total_height + child->margin().top()
					        + child->size().height() + child->margin().bottom();
				}
			}
			total_height += padding_.top();

			std::list<Drawable*>::const_iterator it;
			for (it = m_list.begin(); it != m_list.end(); it++) {
				child = *it;
				if (child) {
					if (alignment_ & AlignLeft) {
						child->set_pos(
						        pos_.x() + padding_.left()
						                + child->margin().left(),
						        child->pos().y());
					} else if (alignment_ & AlignRight) {
						child->set_pos(
						        pos_.x()
						                + (total_width
						                        - (padding_.right()
						                                + child->size().width()
						                                + child->margin().right())),
						        child->pos().y());
					} else if (alignment_ & AlignVerticalCenter) {
						child->set_pos(
						        pos_.x() + padding_.left()
						                + child->margin().left()
						                + (max_widget_width
						                        - child->size().width()) / 2,
						        child->pos().y());
					}
				}
			}

			size_.set_width(total_width);
			size_.set_height(total_height);
		//}
	}

	void VerticalLayout::render ()
	{

		list<Drawable*>::const_iterator it;
		Drawable *item = 0;
		for (it = m_list.begin(); it != m_list.end(); it++) {
			item = *it;
			if (item) {
				Interface::instance()->dispatch_render_event(item);
			}
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(), pos_.y(), z());
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

	void VerticalLayout::press_key (KeyEvent* event)
	{

	}

	void VerticalLayout::press_context_menu (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::release_context_menu (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::press_mouse (MouseEvent* event)
	{
		list<Drawable*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void VerticalLayout::release_mouse (MouseEvent* event)
	{
		list<Drawable*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void VerticalLayout::move_mouse (MouseEvent* event)
	{
		list<Drawable*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

}
