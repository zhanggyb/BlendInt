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

#include <queue>
#include <iostream>

#include <BILO/HorizontalLayout.hpp>

#include <BILO/Interface.hpp>

namespace BILO {

	HorizontalLayout::HorizontalLayout(int align)
		: AbstractLayout()
	{
		set_alignment(align);
	}

	HorizontalLayout::HorizontalLayout(int align, Drawable* parent)
		: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	HorizontalLayout::~HorizontalLayout()
	{

	}

	bool HorizontalLayout::update (int type, const void* property)
	{
		if(m_parent.type == ParentDrawable) {
			const Point* new_pos = &m_pos;
			if (type == WidgetPropertyPosition)
				new_pos = static_cast<const Point*>(property);

			//if (property == WidgetPropertySize) {
			unsigned int total_width = 0;
			unsigned int total_height = 0;
			unsigned int max_widget_height = 0;

			std::vector<Drawable*>::const_iterator it;
			Drawable* child = 0;
			total_width = m_margin.left();
			for (it = m_vector.begin(); it != m_vector.end(); it++) {
				child = *it;
				set_pos_priv(child, new_pos->x() + total_width,
				        new_pos->y() + m_margin.bottom());
				total_width = total_width + child->size().width();
				total_height = std::max(total_height,
				        m_margin.top() + child->size().height() + m_margin.bottom());
				max_widget_height = std::max(max_widget_height,
				        child->size().height());
				total_width += m_space;
			}

			total_width = total_width - m_space + m_margin.right();

			for (it = m_vector.begin(); it != m_vector.end(); it++) {
				child = *it;

				if (m_alignment & AlignTop) {
					set_pos_priv(child, child->pos().x(),
					        new_pos->y()
					                + (total_height
					                        - (m_margin.top()
					                                + child->size().height())));
				} else if (m_alignment & AlignBottom) {
					set_pos_priv(child, child->pos().x(),
					        new_pos->y()
					                + m_margin.bottom());
				} else if (m_alignment & AlignHorizontalCenter) {
					set_pos_priv(child, child->pos().x(),
					        new_pos->y() + m_margin.bottom()
					                + (max_widget_height - child->size().height())
					                        / 2 );
				}

			}

			m_size.set_width(total_width);
			m_size.set_height(total_height);
			//}

			return true;

		} else {

			if (type == WidgetPropertySize) {

				if (property) {
					generate_layout(static_cast<const Size*>(property));
				} else {
					reset_size_default();
				}

			}
		}

		return true;
	}

	void HorizontalLayout::render ()
	{
		std::vector<Drawable*>::const_iterator it;
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
				Interface::instance()->dispatch_render_event(*it);
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());
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
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
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
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void HorizontalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void HorizontalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

	void HorizontalLayout::generate_layout (const Size* size)
	{
		std::queue<Drawable*> expandable_objects;
		std::queue<Drawable*> unexpandable_objects;

		std::vector<Drawable*>::iterator it;
		Drawable* child = 0;

		int unchangeable_width = 0;
		unsigned int total_height = size->height();
		unsigned int max_widget_height = total_height - m_margin.top() - m_margin.bottom();

		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			child = *it;
			if(child->hexpand()) {
				expandable_objects.push(child);
			} else {
				unexpandable_objects.push(child);
				unchangeable_width += child->size().width();
			}
			total_height = std::max(total_height,
			        m_margin.top() + child->size().height() + m_margin.bottom());
			max_widget_height = std::max (max_widget_height, child->size().height());
		}

		int flexible_with = size->width() - m_margin.left() - m_margin.right() - (m_vector.size() - 1) * m_space - unchangeable_width;
		int single_flexible_with = flexible_with / expandable_objects.size();

		while(!expandable_objects.empty()) {
			child = expandable_objects.front();
			resize_priv (child, single_flexible_with, child->size().height());
			expandable_objects.pop();
		}

		Point pos = m_pos;
		pos.set_x(pos.x() + m_margin.left());
		pos.set_y(pos.y() + m_margin.bottom());

		std::cout << size->width() << " " << size->height() << std::endl;


		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			child = *it;

			// set position
			set_pos_priv(child, pos);

			std::cout << child->pos().x() << " " << child->pos().y() << std::endl;

			// set height
			if (child->vexpand()) {
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

	void HorizontalLayout::reset_size_default ()
	{
		unsigned int total_width = 0;
		unsigned int total_height = m_size.height();
		unsigned int max_widget_height = 0;

		std::vector<Drawable*>::const_iterator it;
		Drawable* child = 0;
		total_width = m_margin.left();
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			child = *it;
			set_pos_priv(child, m_pos.x() + total_width,
			        m_pos.y() + m_margin.bottom());
			total_width = total_width + child->size().width();
			total_height = std::max(total_height,
			        m_margin.top() + child->size().height() + m_margin.bottom());
			max_widget_height = std::max(max_widget_height,
			        child->size().height());
			total_width += m_space;
		}
		total_width = total_width - m_space + m_margin.right();

		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			child = *it;

			if (child->vexpand()) {
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
					        m_pos.y() + (total_height
					                        - child->size().height()) / 2);
				}
			}

		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);
	}

}

