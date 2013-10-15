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
		switch(type) {
			case BasicPropertyPosition: {
				const Point* new_pos = static_cast<const Point*>(property);

				for (size_t i = 0; i < m_items.size(); i++)
				{
					set_pos_priv(m_items[i], m_items[i]->pos().x() + (new_pos->x() - m_pos.x()),
							m_items[i]->pos().y() + (new_pos->y() - m_pos.y()));
				}
				return true;
			}

			case BasicPropertySize: {
				if(expand_x()) {
					generate_layout(static_cast<const Size*>(property));
					return true;
				} else {
 					generate_default_layout();
 					return false;
				}
			}

			case LayoutPropertyItem: {
				if(m_in_layout) {

					AbstractLayout* root_layout = dynamic_cast<AbstractLayout*>(m_parent.object.drawable);
					while(root_layout->in_layout()) {
						root_layout = dynamic_cast<AbstractLayout*>(root_layout->parent()->object.drawable);
						if(!root_layout) break;
					}

					if(root_layout) {
						root_layout->refresh();
					}

				} else {
					generate_default_layout();
				}

				return true;
			}

			default:
				break;
		}

		return true;
	}

	void HorizontalLayout::render ()
	{
		std::vector<Drawable*>::const_iterator it;
		for (it = m_items.begin(); it != m_items.end(); it++) {
			Interface::instance()->dispatch_render_event(*it);
		}
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

	void HorizontalLayout::generate_layout (const Size* size)
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
			total_height = std::max(total_height,
			        m_margin.top() + child->size().height()
			                + m_margin.bottom());
			max_widget_height = std::max(max_widget_height,
			        child->size().height());
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

			if(i != (m_items.size() - 1))
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

		for(size_t i = 0; i < m_items.size(); i++)
		{
			child = m_items[i];
			if(child->expand_x()) {
				minimal_size.add_width(child->minimal_size().width());
			} else {
				minimal_size.add_width(child->size().width());
			}

			if(child->expand_y()) {
				minimal_size.set_height(std::max(minimal_size.height(), child->minimal_size().height()));
			} else {
				minimal_size.set_height(std::max(minimal_size.height(), child->size().height()));
			}

			if(i != (m_items.size() - 1))
				minimal_size.add_width(m_space);
		}
		minimal_size.add_width(m_margin.right());
		minimal_size.add_height(m_margin.top() + m_margin.bottom());

		return minimal_size;
	}

}

