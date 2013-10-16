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

#include <GL/glew.h>
#include <algorithm>
#include <queue>
#include <iostream>

#include <BlendInt/VerticalLayout.hpp>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

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

	bool VerticalLayout::update (int type, const void* property)
	{
		switch (type) {

			case BasicPropertySize: {
				change_layout(static_cast<const Size*>(property));
				return true;
			}

			case LayoutPropertyItem: {
				const ItemData* item = static_cast<const ItemData*>(property);

				if (item->action) {	// Add item
					add_item(item->object);
				} else { // remove item
				}

				return true;
			}

			default:
				return AbstractLayout::update(type, property);
		}
	}

	void VerticalLayout::render ()
	{

		std::vector<Drawable*>::const_iterator it;
		Drawable *item = 0;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			item = *it;
			if (item) {
				Interface::instance()->dispatch_render_event(item);
			}
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(), m_pos.y(), z());

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(m_size.width(), 0);
		glVertex2i(m_size.width(), m_size.height());
		glVertex2i(0, m_size.height());
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
		std::vector<Drawable*>::iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void VerticalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void VerticalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

	void VerticalLayout::change_layout (const Size* size)
	{
		std::queue<Drawable*> expandable_objects;
		std::queue<Drawable*> unexpandable_objects;

		std::vector<Drawable*>::reverse_iterator it;
		Drawable* child = 0;

		int fixed_height = 0;
		unsigned int total_width = size->width();
		unsigned int max_widget_width = total_width - m_margin.left()
		        - m_margin.right();

		for (it = m_items.rbegin(); it != m_items.rend(); it++) {
			child = *it;
			if (child->expand_y()) {
				expandable_objects.push(child);
			} else {
				unexpandable_objects.push(child);
				fixed_height += child->size().height();
			}
//			total_width = std::max(total_width,
//			        m_margin.left() + child->size().width() + m_margin.right());
//			max_widget_width = std::max(max_widget_width,
//			        child->size().width());
		}

		int flexible_height = size->height() - m_margin.top()
		        - m_margin.bottom() - (m_items.size() - 1) * m_space
		        - fixed_height;

		if (expandable_objects.size() > 0) {

			int single_flexible_height = flexible_height
			        / expandable_objects.size();

			while (!expandable_objects.empty()) {
				child = expandable_objects.front();
				resize_priv(child, child->size().width(),
				        single_flexible_height);
				expandable_objects.pop();
			}
		}

		Point pos = m_pos;
		pos.set_x(pos.x() + m_margin.left());
		pos.set_y(pos.y() + m_margin.bottom());

		for (it = m_items.rbegin(); it != m_items.rend(); it++) {
			child = *it;

			// set position
			set_pos_priv(child, pos);

			// set width
			if (child->expand_x()) {
				resize_priv(child, max_widget_width, child->size().height());
			} else {
				if (m_alignment & AlignLeft) {
					set_pos_priv(child, m_pos.x() + m_margin.left(),
					        child->pos().y());
				} else if (m_alignment & AlignRight) {
					set_pos_priv(child,
					        m_pos.x()
					                + (total_width
					                        - (m_margin.right()
					                                + child->size().width())),
					        child->pos().y());
				} else if (m_alignment & AlignVerticalCenter) {

					set_pos_priv(child,
					        m_pos.x() + m_margin.right()
					                + (max_widget_width - child->size().width())
					                        / 2, child->pos().y());
				}
			}

			pos.set_y(pos.y() + child->size().height() + m_space);
		}

		return;
	}

	void VerticalLayout::generate_default_layout ()
	{
		unsigned int total_width = m_size.width();
		unsigned int total_height = 0;
		unsigned int max_widget_width = 0;

		std::vector<Drawable*>::const_reverse_iterator it;
		Drawable* child = 0;
		total_height = m_margin.bottom();
		for (it = m_items.rbegin(); it != m_items.rend(); it++) {
			child = *it;
			set_pos_priv(child, m_pos.x() + m_margin.left(),
			        m_pos.y() + total_height);
			total_width = std::max(total_width,
			        m_margin.left() + child->size().width() + m_margin.right());
			total_height = total_height + child->size().height();
			max_widget_width = std::max(max_widget_width,
			        child->size().width());
			total_height += m_space;
		}
		total_height = total_height - m_space + m_margin.top();

		for (it = m_items.rbegin(); it != m_items.rend(); it++) {
			child = *it;

			if (child->expand_x()) {
				resize_priv(child, max_widget_width, child->size().height());
			} else {
				if (m_alignment & AlignLeft) {
					set_pos_priv(child, m_pos.x() + m_margin.left(),
					        child->pos().y());
				} else if (m_alignment & AlignRight) {
					set_pos_priv(child,
					        m_pos.x()
					                + (total_width
					                        - (m_margin.right()
					                                + child->size().width())),
					        child->pos().y());
				} else if (m_alignment & AlignVerticalCenter) {
					set_pos_priv(child,
					        m_pos.x()
					                + (total_width - child->size().width()) / 2,
					        child->pos().y());
				}
			}

		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);
	}

	Size VerticalLayout::get_minimal_size()
	{
		Size minimal_size;

		Drawable* child;
		minimal_size.add_height(m_margin.top());

		for(size_t i = 0; i < m_items.size(); i++)
		{
			child = m_items[i];
			if(child->expand_y()) {
				minimal_size.add_height(child->minimal_size().height());
			} else {
				minimal_size.add_height(child->size().height());
			}

			if(child->expand_x()) {
				minimal_size.set_width(std::max(minimal_size.width(), child->minimal_size().width()));
			} else {
				minimal_size.set_width(std::max(minimal_size.width(), child->size().width()));
			}

			if(i != (m_items.size() - 1))
				minimal_size.add_height(m_space);
		}
		minimal_size.add_height(m_margin.bottom());
		minimal_size.add_width(m_margin.left() + m_margin.right());

		return minimal_size;
	}

	void VerticalLayout::add_item (Drawable* object)
	{
		unsigned int inner_width = m_size.width() - m_margin.left()
		        - m_margin.right();
		unsigned int inner_height = m_size.height() - m_margin.top()
		        - m_margin.bottom();

		inner_width = std::max(inner_width, object->size().width());
		inner_height = inner_height + object->size().height();

		if (m_items.size() == 0) {
			set_pos_priv(object, m_pos.x() + m_margin.left(),
			        m_pos.y() + m_margin.bottom());
			m_size.set_height(m_margin.top() + inner_height + m_margin.bottom());
			m_minimal_size.add_height(object->minimal_size().height());
		} else {
			set_pos_priv(object,
			        m_pos.x() + m_margin.left(),
			        m_pos.y() - (object->size().height() + m_space));
			m_size.set_height(
			        m_margin.top() + inner_height + m_space + m_margin.bottom());
			m_minimal_size.add_height(object->minimal_size().height() + m_space);
		}

		m_size.set_width(m_margin.left() + inner_width + m_margin.right());
		m_minimal_size.set_width(std::max(m_minimal_size.width(), object->minimal_size().width()));

		if(!expand_x())
			set_expand_x(object->expand_x());

		if(!expand_y())
			set_expand_y(object->expand_y());

		m_items.push_back(object);
		bind(object);

		align_along_y(inner_width);

		if(m_in_layout) {
			dynamic_cast<AbstractLayout*>(m_parent.object.drawable)->refresh();
		}
	}

	void VerticalLayout::align_along_y (unsigned int width)
	{
		Drawable* child = 0;
		std::vector<Drawable*>::iterator it;

		int y = m_pos.y() + m_size.height() - m_margin.top();
		for (it = m_items.begin(); it != m_items.end(); it++) {
			child = *it;

			if(it == m_items.begin())
				y = y - child->size().height();
			else
				y = y - child->size().height() - m_space;

			if (child->expand_x()) {
				resize_priv(child, width, child->size().height());
			}

			if (m_alignment & AlignLeft) {
				set_pos_priv(child, m_pos.x() + m_margin.left(), y);
			} else if (m_alignment & AlignRight) {
				set_pos_priv(child,
				        m_pos.x() + m_margin.left()
				                + (width - child->size().width()), y);
			} else if (m_alignment & AlignVerticalCenter) {
				set_pos_priv(child,
				        m_pos.x() + m_margin.left()
				                + (width - child->size().width()) / 2, y);
			}
		}
	}

	Size VerticalLayout::calculate_size()
	{
		Size size;

		std::vector<Drawable*>::iterator it;

		for(it = m_items.begin(); it != m_items.end(); it++)
		{
			size.set_width(std::max((*it)->size().width(), size.width()));
			size.add_height((*it)->size().height());
		}

		size.add_height(m_space * (m_items.size() - 1) + m_margin.top() + m_margin.bottom());
		size.add_width(m_margin.left() + m_margin.right());

		return size;
	}

}
