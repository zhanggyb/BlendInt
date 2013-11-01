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

	VerticalLayout::VerticalLayout (int align, AbstractForm* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	VerticalLayout::~VerticalLayout ()
	{

	}

	void VerticalLayout::update (int property_type)
	{
		switch (property_type) {

			case FormPropertySize: {
				if(items().size())
					make_layout();
				return;
			}

			case LayoutPropertyItem: {
				return;
			}

			default:
				break;
		}
	}

	void VerticalLayout::render ()
	{

		std::vector<AbstractForm*>::const_iterator it;
		AbstractForm *item = 0;
		for (it = items().begin(); it != items().end(); it++) {
			item = *it;
			if (item) {
				Interface::instance()->dispatch_render_event(item);
			}
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

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
		std::vector<AbstractForm*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			Interface::instance()->dispatch_key_press_event(*it, event);
		}
	}

	void VerticalLayout::press_context_menu (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::release_context_menu (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::press_mouse (MouseEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void VerticalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void VerticalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items().begin(); it != items().end(); it++) {
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

	void VerticalLayout::add_item (Widget* widget)
	{
		// don't fire events when adding a widget into a layout
		widget->deactivate_events();
		deactivate_events();

		Size min = minimal_size();
		Size preferred = preferred_size();
		Size current_size = size();

		unsigned int w_plus = margin().left() + margin().right();

		if (items().size() == 0) {
			min.add_height(widget->minimal_size().height());
			preferred.add_height(widget->preferred_size().height());
		} else {
			min.add_height(widget->minimal_size().height() + space());
			preferred.add_height(widget->preferred_size().height() + space());
		}

		min.set_width(
		        std::max(min.width(), widget->minimal_size().width() + w_plus));
		preferred.set_width(
		        std::max(preferred.width(),
		                widget->preferred_size().width() + w_plus));

		if (current_size.width() < preferred.width()) {
			current_size.set_width(preferred.width());
		}
		if (current_size.height() < preferred.height()) {
			current_size.set_height(preferred.height());
		}

		items().push_back(widget);

		set_preferred_size(preferred);
		set_minimal_size(min);

		if(widget->expand_y()) m_expandable_items.insert(widget);
		else m_fixed_items.insert(widget);

		if(! (current_size == size()))
			resize_priv(this, current_size);	// call make_layout() through this function
		else
			make_layout();

		activate_events();
		widget->activate_events();

		bind(widget);
		set_in_layout(widget, true);
	}

	void VerticalLayout::add_item (AbstractLayout* layout)
	{

	}

	void VerticalLayout::remove_item(AbstractForm * object)
	{
		deactivate_events();

		std::vector<AbstractForm*>::iterator it;
		for(it = items().begin(); it != items().end();)
		{
			if ((*it) == object) {
				it = items().erase(it);
			} else {
				it++;
			}
		}

		if(object->expand_y())
				m_expandable_items.erase(object);
		else
			m_fixed_items.erase(object);

		Size new_preferred_size;
		Size new_minimal_size;

		get_size_hint(true, true, 0, &new_minimal_size, &new_preferred_size);

		set_minimal_size(new_minimal_size);
		set_preferred_size(new_preferred_size);

		make_layout();

		activate_events();

		set_in_layout(object, false);
		unbind(object);
	}

	void VerticalLayout::make_layout()
	{
		if (size().height() == preferred_size().height()) {
			distribute_with_preferred_size();			// layout along y with preferred size
		} else if (size().height() < preferred_size().height()) {
			distribute_with_small_size();			// layout along y with small size
		} else {
			distribute_with_large_size();			// layout along y with large size
		}

		align();
	}

	void VerticalLayout::distribute_with_preferred_size()
	{
		int y = position().y() + size().height() - margin().top();

		std::vector<AbstractForm*>::reverse_iterator it;
		AbstractForm* child = 0;
		for(it = items().rbegin(); it != items().rend(); it++)
		{
			if(! (it == items().rbegin()))
				y -= space();

			child = *it;
			resize_priv(child, child->size().width(), child->preferred_size().height());
			y -= child->size().height();
			set_pos_priv(child, child->position().x(), y);
		}
	}

	void VerticalLayout::distribute_with_small_size()
	{

	}

	void VerticalLayout::distribute_with_large_size()
	{
		unsigned int fixed_h = fixed_height();

		unsigned int current_height = size().height();

		unsigned int h_plus = margin().top() + margin().bottom();

		std::vector<AbstractForm*>::reverse_iterator it;
		AbstractForm* child = 0;
		int y = position().y() + size().height() - margin().top();

		unsigned int single_height = current_height - h_plus - fixed_h - (items().size() - 1) * space();

		if(m_expandable_items.size())
			single_height = single_height / m_expandable_items.size();

		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			if (!(it == items().rbegin()))
				y -= space();

			if (m_expandable_items.count(child)) {
				resize_priv(child, child->size().width(), single_height);
			} else {
				resize_priv(child, child->size().width(), child->preferred_size().height());
			}

			y -= child->size().height();
			set_pos_priv(child, child->position().x(), y);
		}
	}

	void VerticalLayout::align()
	{
		int x = position().x() + margin().left();

		unsigned int w = size().width() - margin().left() - margin().right();

		std::vector<AbstractForm*>::reverse_iterator it;
		AbstractForm* child = 0;
		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			if (child->expand_x() ||
					(child->size().width() > w)) {
				resize_priv(child, w, child->size().height());
				set_pos_priv(child, x, child->position().y());
			} else {

				if (alignment() & AlignLeft) {
					set_pos_priv(child, x, child->position().y());
				} else if (alignment() & AlignRight) {
					set_pos_priv(child,
							x + (w - child->size().width()), child->position().y());
				} else if (alignment() & AlignVerticalCenter) {
					set_pos_priv(child,
					        x + (w - child->size().width()) / 2, child->position().y());
				}

			}
		}

	}

	void VerticalLayout::reset_height_of_fixed_items (std::set<AbstractForm*>* items,
													  unsigned int height)
	{
		std::set<AbstractForm*>::iterator it;

		for(it = items->begin(); it != items->end(); it++)
		{
			resize_priv(*it, (*it)->size().width(), height);
		}
	}

	unsigned int VerticalLayout::minimal_expandable_height()
	{
		unsigned int height = 0;

		std::set<AbstractForm*>::iterator it;
		for(it = m_expandable_items.begin(); it != m_expandable_items.end(); it++)
		{
			height += (*it)->minimal_size().height();
		}

		return height;
	}

	unsigned int VerticalLayout::fixed_height()
	{
		unsigned int height = 0;

		std::set<AbstractForm*>::iterator it;
		for(it = m_fixed_items.begin(); it != m_fixed_items.end(); it++)
		{
			height += (*it)->size().height();
		}

		return height;
	}

	void VerticalLayout::get_size_hint (bool count_margin,
										bool count_space,
										Size* size,
										Size* min,
										Size* preferred)
	{
		Size size_out;
		Size min_size_out;
		Size preferred_size_out;

		AbstractForm* child;
		std::vector<AbstractForm*>::reverse_iterator it;

		if(count_margin) {
			size_out.set_height(margin().top());
			min_size_out.set_height(margin().top());
			preferred_size_out.set_height(margin().top());
		}

		for(it = items().rbegin(); it != items().rend(); it++)
		{
			child = *it;

			size_out.set_width(std::max(size_out.width(), child->size().width()));
			size_out.add_height(child->size().height());

			min_size_out.set_width(std::max(min_size_out.width(), child->minimal_size().width()));
			min_size_out.add_height(child->minimal_size().height());

			preferred_size_out.set_width(std::max(preferred_size_out.width(), child->preferred_size().width()));
			preferred_size_out.add_height(child->preferred_size().height());
		}

		if(count_margin) {
			size_out.add_height(margin().bottom());
			size_out.add_width(margin().left() + margin().right());

			min_size_out.add_height(margin().bottom());
			min_size_out.add_width(margin().left() + margin().right());

			preferred_size_out.add_height(margin().bottom());
			preferred_size_out.add_width(margin().left() + margin().right());
		}

		if(count_space) {
			size_out.add_height((items().size() - 1) * space());

			min_size_out.add_height((items().size() - 1) * space());

			preferred_size_out.add_height((items().size() - 1) * space());
		}

		if(size) *size = size_out;
		if(min) *min = min_size_out;
		if(preferred) *preferred = preferred_size_out;
	}

	void VerticalLayout::change_layout (const Size* size)
	{
		std::queue<AbstractForm*> expandable_objects;
		std::queue<AbstractForm*> unexpandable_objects;

		std::vector<AbstractForm*>::reverse_iterator it;
		AbstractForm* child = 0;

		int fixed_height = 0;
		unsigned int total_width = size->width();
		unsigned int max_widget_width = total_width - margin().left()
		        - margin().right();

		for (it = items().rbegin(); it != items().rend(); it++) {
			child = *it;
			if (child->expand_y()) {
				expandable_objects.push(child);
			} else {
				unexpandable_objects.push(child);
				fixed_height += child->size().height();
			}
//			total_width = std::max(total_width,
//			        margin().left() + child->size().width() + margin().right());
//			max_widget_width = std::max(max_widget_width,
//			        child->size().width());
		}

		int flexible_height = size->height() - margin().top()
		        - margin().bottom() - (items().size() - 1) * space()
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

		Point pos = position();
		pos.set_x(pos.x() + margin().left());
		pos.set_y(pos.y() + margin().bottom());

		for (it = items().rbegin(); it != items().rend(); it++) {
			child = *it;

			// set position
			set_pos_priv(child, pos);

			// set width
			if (child->expand_x()) {
				resize_priv(child, max_widget_width, child->size().height());
			} else {
				if (alignment() & AlignLeft) {
					set_pos_priv(child, position().x() + margin().left(),
					        child->position().y());
				} else if (alignment() & AlignRight) {
					set_pos_priv(child,
					        position().x()
					                + (total_width
					                        - (margin().right()
					                                + child->size().width())),
					        child->position().y());
				} else if (alignment() & AlignVerticalCenter) {

					set_pos_priv(child,
					        position().x() + margin().right()
					                + (max_widget_width - child->size().width())
					                        / 2, child->position().y());
				}
			}

			pos.set_y(pos.y() + child->size().height() + space());
		}

		return;
	}

	void VerticalLayout::generate_default_layout ()
	{
		unsigned int total_width = m_size.width();
		unsigned int total_height = 0;
		unsigned int max_widget_width = 0;

		std::vector<AbstractForm*>::const_reverse_iterator it;
		AbstractForm* child = 0;
		total_height = margin().bottom();
		for (it = items().rbegin(); it != items().rend(); it++) {
			child = *it;
			set_pos_priv(child, position().x() + margin().left(),
			        position().y() + total_height);
			total_width = std::max(total_width,
			        margin().left() + child->size().width() + margin().right());
			total_height = total_height + child->size().height();
			max_widget_width = std::max(max_widget_width,
			        child->size().width());
			total_height += space();
		}
		total_height = total_height - space() + margin().top();

		for (it = items().rbegin(); it != items().rend(); it++) {
			child = *it;

			if (child->expand_x()) {
				resize_priv(child, max_widget_width, child->size().height());
			} else {
				if (alignment() & AlignLeft) {
					set_pos_priv(child, position().x() + margin().left(),
					        child->position().y());
				} else if (alignment() & AlignRight) {
					set_pos_priv(child,
					        position().x()
					                + (total_width
					                        - (margin().right()
					                                + child->size().width())),
					        child->position().y());
				} else if (alignment() & AlignVerticalCenter) {
					set_pos_priv(child,
					        position().x()
					                + (total_width - child->size().width()) / 2,
					        child->position().y());
				}
			}

		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);
	}

	Size VerticalLayout::get_minimal_size()
	{
		Size minimal_size;

		AbstractForm* child;
		minimal_size.add_height(margin().top());

		for(size_t i = 0; i < items().size(); i++)
		{
			child = items()[i];
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

			if(i != (items().size() - 1))
				minimal_size.add_height(space());
		}
		minimal_size.add_height(margin().bottom());
		minimal_size.add_width(margin().left() + margin().right());

		return minimal_size;
	}

	void VerticalLayout::align_along_y (unsigned int width)
	{
		AbstractForm* child = 0;
		std::vector<AbstractForm*>::iterator it;

		int y = position().y() + m_size.height() - margin().top();
		for (it = items().begin(); it != items().end(); it++) {
			child = *it;

			if(it == items().begin())
				y = y - child->size().height();
			else
				y = y - child->size().height() - space();

			if (child->expand_x()) {
				resize_priv(child, width, child->size().height());
			}

			if (alignment() & AlignLeft) {
				set_pos_priv(child, position().x() + margin().left(), y);
			} else if (alignment() & AlignRight) {
				set_pos_priv(child,
				        position().x() + margin().left()
				                + (width - child->size().width()), y);
			} else if (alignment() & AlignVerticalCenter) {
				set_pos_priv(child,
				        position().x() + margin().left()
				                + (width - child->size().width()) / 2, y);
			}
		}
	}

}
