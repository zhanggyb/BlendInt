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
		if (m_parent.type == ParentDrawable) {
			const Point* new_pos = &m_pos;
			if (type == BasicPropertyPosition)
				new_pos = static_cast<const Point*>(property);

			unsigned int total_width = 0;
			unsigned int total_height = 0;
			unsigned int max_widget_width = 0;

			std::vector<Drawable*>::const_reverse_iterator rit;
			Drawable* child = 0;
			total_height = m_margin.bottom();
			for (rit = m_vector.rbegin(); rit != m_vector.rend(); rit++) {
				child = *rit;

				set_pos_priv(child, new_pos->x() + m_margin.left(),
				        new_pos->y() + total_height);
				total_width = std::max(total_width,
				        m_margin.left() + child->size().width()
				                + m_margin.right());
				max_widget_width = std::max(max_widget_width,
				        child->size().width());
				total_height = total_height + child->size().height();

			}
			total_height += m_margin.top();

			std::vector<Drawable*>::const_iterator it;
			for (it = m_vector.begin(); it != m_vector.end(); it++) {
				child = *it;

				if (m_alignment & AlignLeft) {
					set_pos_priv(child, new_pos->x() + m_margin.left(),
					        child->pos().y());
				} else if (m_alignment & AlignRight) {
					set_pos_priv(child,
					        new_pos->x()
					                + (total_width
					                        - (m_margin.right()
					                                + child->size().width())),
					        child->pos().y());
				} else if (m_alignment & AlignVerticalCenter) {
					set_pos_priv(child,
					        new_pos->x() + m_margin.left()
					                + (max_widget_width - child->size().width())
					                        / 2, child->pos().y());
				}

			}

			m_size.set_width(total_width);
			m_size.set_height(total_height);

			return true;
		} else {

			if (type == BasicPropertySize) {

				if (property) {
					generate_layout(static_cast<const Size*>(property));
				} else {
					generate_default_layout();
				}

				return true;
			}
		}

		return true;
	}

	void VerticalLayout::render ()
	{

		std::vector<Drawable*>::const_iterator it;
		Drawable *item = 0;
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
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
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void VerticalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void VerticalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

	void VerticalLayout::generate_layout(const Size* size)
	{
		std::queue<Drawable*> expandable_objects;
		std::queue<Drawable*> unexpandable_objects;

		std::vector<Drawable*>::reverse_iterator it;
		Drawable* child = 0;

		int fixed_height = 0;
		unsigned int total_width = size->width();
		unsigned int max_widget_width = total_width - m_margin.left() - m_margin.right();

		for(it = m_vector.rbegin(); it != m_vector.rend(); it++)
		{
			child = *it;
			if(child->expand_y()) {
				expandable_objects.push(child);
			} else {
				unexpandable_objects.push(child);
				fixed_height += child->size().height();
			}
			total_width = std::max(total_width,
			        m_margin.left() + child->size().width() + m_margin.right());
			max_widget_width = std::max (max_widget_width, child->size().width());
		}

		int flexible_height = size->height() - m_margin.top() - m_margin.bottom() - (m_vector.size() - 1) * m_space - fixed_height;

		if (expandable_objects.size() > 0) {

			int single_flexible_height = flexible_height / expandable_objects.size();

			while(!expandable_objects.empty()) {
				child = expandable_objects.front();
				resize_priv (child, child->size().width(), single_flexible_height);
				expandable_objects.pop();
			}
		}

		Point pos = m_pos;
		pos.set_x(pos.x() + m_margin.left());
		pos.set_y(pos.y() + m_margin.bottom());

		for (it = m_vector.rbegin(); it != m_vector.rend(); it++) {
			child = *it;

			// set position
			set_pos_priv(child, pos);

			// set width
			if (child->expand_x()) {
				resize_priv(child, max_widget_width, child->size().height());
			} else {
				if (m_alignment & AlignLeft) {
					set_pos_priv(child, m_pos.x() + m_margin.left(), child->pos().y());
				} else if (m_alignment & AlignRight) {
					set_pos_priv(child,
					        m_pos.x()
					                + (total_width
					                        - (m_margin.right()
					                                + child->size().width())),
					                                child->pos().y());
				} else if (m_alignment & AlignVerticalCenter) {

					set_pos_priv(child, m_pos.x() + m_margin.right() + (max_widget_width - child->size().width()) / 2,
							child->pos().y());
				}
			}

			pos.set_y(pos.y() + child->size().height() + m_space);
		}

		return;
	}

	void VerticalLayout::generate_default_layout()
	{
		unsigned int total_width = m_size.width();
		unsigned int total_height = 0;
		unsigned int max_widget_width = 0;

		std::vector<Drawable*>::const_reverse_iterator it;
		Drawable* child = 0;
		total_height = m_margin.bottom();
		for (it = m_vector.rbegin(); it != m_vector.rend(); it++) {
			child = *it;
			set_pos_priv(child, m_pos.x() + m_margin.left(),
			        m_pos.y() + total_height);
			total_width = std::max(total_width, m_margin.left() + child->size().width() + m_margin.right());
			total_height = total_height + child->size().height();
			max_widget_width = std::max(max_widget_width,
			        child->size().width());
			total_height += m_space;
		}
		total_height = total_height - m_space + m_margin.top();

		for (it = m_vector.rbegin(); it != m_vector.rend(); it++) {
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

}
