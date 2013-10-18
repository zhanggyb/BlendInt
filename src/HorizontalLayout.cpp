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

#include <BlendInt/HorizontalLayout.hpp>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

	HorizontalLayout::HorizontalLayout (int align)
			: AbstractLayout()
	{
		set_alignment(align);
	}

	HorizontalLayout::HorizontalLayout (int align, Drawable* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	HorizontalLayout::~HorizontalLayout ()
	{

	}

	bool HorizontalLayout::update (int type, const void* property)
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
					remove_item(item->object);
				}

				return true;
			}

			default:
				return AbstractLayout::update(type, property);
		}
	}

	void HorizontalLayout::render ()
	{
		std::vector<Drawable*>::const_iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_render_event(*it);
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

	void HorizontalLayout::press_key (KeyEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_key_press_event(*it, event);
		}
	}

	void HorizontalLayout::press_context_menu (ContextMenuEvent* event)
	{
	}

	void HorizontalLayout::release_context_menu (ContextMenuEvent* event)
	{
	}

	void HorizontalLayout::press_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void HorizontalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void HorizontalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

	void HorizontalLayout::change_layout (const Size* size)
	{
		std::queue<Drawable*> expandable_objects;
		std::queue<Drawable*> unexpandable_objects;

		Drawable* child = 0;

		// first, classify objects in layout according to "hexpand" property
		int fixed_width = 0;
		unsigned int total_height = size->height();
		unsigned int max_widget_height = total_height - m_margin.top()
		        - m_margin.bottom();

		for (size_t i = 0; i < m_items.size(); i++) {
			child = m_items[i];
			if (child->expand_x()) {
				expandable_objects.push(child);
			} else {
				unexpandable_objects.push(child);
				fixed_width += child->size().width();
			}
//			total_height = std::max(total_height,
//			        m_margin.top() + child->size().height()
//			                + m_margin.bottom());
//			max_widget_height = std::max(max_widget_height,
//			        child->size().height());
		}

		// average the width of each expandable object along horizontal
		if (expandable_objects.size() > 0) {
			int flexible_width = size->width() - m_margin.left()
			        - m_margin.right() - (m_items.size() - 1) * m_space
			        - fixed_width;
			int single_flexible_with = flexible_width
			        / expandable_objects.size();

			while (!expandable_objects.empty()) {
				child = expandable_objects.front();
				resize_priv(child, single_flexible_with,
				        child->size().height());
				expandable_objects.pop();
			}
		}

		// then move each object to the right place
		Point pos = m_pos;
		pos.set_x(pos.x() + m_margin.left());
		pos.set_y(pos.y() + m_margin.bottom());

		for (size_t i = 0; i < m_items.size(); i++) {
			child = m_items[i];

			// set position
			set_pos_priv(child, pos);

			// set height
			if (child->expand_y()) {
				resize_priv(child, child->size().width(), max_widget_height);
			} else {
				if (m_alignment & AlignTop) {
					set_pos_priv(child, child->pos().x(),
					        m_pos.y()
					                + (total_height
					                        - (m_margin.top()
					                                + child->size().height())));
				} else if (m_alignment & AlignBottom) {
					set_pos_priv(child, child->pos().x(),
					        m_pos.y() + m_margin.bottom());
				} else if (m_alignment & AlignHorizontalCenter) {
					set_pos_priv(child, child->pos().x(),
					        m_pos.y() + m_margin.bottom()
					                + (max_widget_height
					                        - child->size().height()) / 2);
				}
			}

			pos.set_x(pos.x() + child->size().width() + m_space);
		}

		return;
	}

	void HorizontalLayout::generate_default_layout ()
	{
		unsigned int total_width = 0;
		unsigned int total_height = m_size.height();
		unsigned int max_widget_height = 0;

		Drawable* child = 0;
		total_width = m_margin.left();
		for (size_t i = 0; i < m_items.size(); i++) {
			child = m_items[i];
			set_pos_priv(child, m_pos.x() + total_width,
			        m_pos.y() + m_margin.bottom());
			total_width = total_width + child->size().width();
			total_height = std::max(total_height,
			        m_margin.top() + child->size().height()
			                + m_margin.bottom());
			max_widget_height = std::max(max_widget_height,
			        child->size().height());

			if (i != (m_items.size() - 1))
				total_width += m_space;
		}
		total_width = total_width + m_margin.right();

		for (size_t i = 0; i < m_items.size(); i++) {
			child = m_items[i];

			if (child->expand_y()) {
				resize_priv(child, child->size().width(), max_widget_height);
			} else {
				if (m_alignment & AlignTop) {
					set_pos_priv(child, child->pos().x(),
					        m_pos.y()
					                + (total_height
					                        - (m_margin.top()
					                                + child->size().height())));
				} else if (m_alignment & AlignBottom) {
					// TODO: not needed as already done in previous loop
					set_pos_priv(child, child->pos().x(),
					        m_pos.y() + m_margin.bottom());
				} else if (m_alignment & AlignHorizontalCenter) {
					set_pos_priv(child, child->pos().x(),
					        m_pos.y()
					                + (total_height - child->size().height())
					                        / 2);
				}
			}

		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);
	}

	Size HorizontalLayout::get_minimal_size ()
	{
		Size minimal_size;

		Drawable* child;
		minimal_size.set_width(m_margin.left());

		for (size_t i = 0; i < m_items.size(); i++) {
			child = m_items[i];
			minimal_size.add_width(child->minimal_size().width());
			minimal_size.set_height(
				        std::max(minimal_size.height(),
				                child->minimal_size().height()));
		}
		minimal_size.add_width((m_items.size() - 1) * m_space + m_margin.right());
		minimal_size.add_height(m_margin.top() + m_margin.bottom());

		return minimal_size;
	}

	void HorizontalLayout::add_item (Drawable* object)
	{
		unsigned int inner_width = m_size.width() - m_margin.left()
		        - m_margin.right();
		unsigned int inner_height = m_size.height() - m_margin.top()
		        - m_margin.bottom();

		inner_width = inner_width + object->size().width();
		inner_height = std::max(object->size().height(), inner_height);

		if (m_items.size() == 0) {
			set_pos_priv(object, m_pos.x() + m_margin.left(),
			        m_pos.y() + m_margin.bottom());
			m_size.set_width(m_margin.left() + inner_width + m_margin.right());

			m_minimal_size.add_width(object->minimal_size().width());
		}	else {
			set_pos_priv(object,
			        m_pos.x() + m_size.width() - m_margin.right() + m_space,
			        m_pos.y() + m_margin.bottom());
			m_size.set_width(
			        m_margin.left() + inner_width + m_space + m_margin.right());
			m_minimal_size.add_width(object->minimal_size().width() + m_space);
		}

		m_size.set_height(m_margin.top() + inner_height + m_margin.bottom());
		m_minimal_size.set_height(std::max(m_minimal_size.height(), object->minimal_size().height()));

		m_items.push_back(object);

		align_along_x(inner_height);	// TODO: no need to pass inner_height

		if(m_in_layout) {
			dynamic_cast<AbstractLayout*>(m_parent.object.drawable)->refresh();
		}
	}

	void HorizontalLayout::remove_item (Drawable* object)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_items.begin(); it != m_items.end();)
		{
			if ((*it) == object) {
				it = m_items.erase(it);
			} else {
				it++;
			}
		}

		m_minimal_size = get_minimal_size();
		m_size = recount_size();

		align_along_x(m_size.height() - m_margin.top() - m_margin.bottom());
	}

	void HorizontalLayout::align_along_x (unsigned int height)
	{
		Drawable* child = 0;
		std::vector<Drawable*>::iterator it;

		m_expand_x = false;
		m_expand_y = false;

		for (it = m_items.begin(); it != m_items.end(); it++) {
			child = *it;

			if (child->expand_y()) {
				resize_priv(child, child->size().width(), height);
			}

			if (m_alignment & AlignTop) {
				set_pos_priv(child, child->pos().x(),
				        m_pos.y() + m_margin.bottom()
				                + (height - child->size().height()));
			} else if (m_alignment & AlignBottom) {
//				set_pos_priv(child, child->pos().x(),
//						child->pos().y() + m_margin.bottom());
			} else if (m_alignment & AlignHorizontalCenter) {
				set_pos_priv(child, child->pos().x(),
				        m_pos.y() + m_margin.bottom()
				                + (height - child->size().height()) / 2);
			}

			// check expand property
			if(!m_expand_x) {
				m_expand_x = child->expand_x() ? true: false;
			}

			if(!m_expand_y) {
				m_expand_y = child->expand_y() ? true: false;
			}

		}

	}

	Size HorizontalLayout::recount_size()
	{
		Size size;

		std::vector<Drawable*>::iterator it;

		for(it = m_items.begin(); it != m_items.end(); it++)
		{
			size.add_width((*it)->size().width());
			size.set_height(std::max((*it)->size().height(), size.height()));
		}

		size.add_width(m_space * (m_items.size() - 1) + m_margin.left() + m_margin.right());
		size.add_height(m_margin.top() + m_margin.bottom());

		return size;
	}

}

