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

	HorizontalLayout::HorizontalLayout (int align, AbstractForm* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	HorizontalLayout::~HorizontalLayout ()
	{

	}

	void HorizontalLayout::update (int property_type)
	{
		switch (property_type) {

			case FormPropertySize: {
				if(items_ref().size())
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

	void HorizontalLayout::render ()
	{
		std::vector<AbstractForm*>::const_iterator it;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			Interface::instance()->dispatch_render_event(*it);
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

	void HorizontalLayout::press_key (KeyEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
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
		std::vector<AbstractForm*>::iterator it;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void HorizontalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void HorizontalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

	void HorizontalLayout::add_item (Widget* object)
	{
		// don't fire events when adding a widget into a layout
		object->deactivate_events();
		deactivate_events();

		Size min = minimal_size();
		Size preferred = preferred_size();
		Size current_size = size();

		unsigned int h_plus = margin().top() + margin().bottom();

		if (items_ref().size() == 0) {
			min.add_width(object->minimal_size().width());
			preferred.add_width(object->preferred_size().width());
		} else {
			min.add_width(object->minimal_size().width() + space());
			preferred.add_width(object->preferred_size().width() + space());
		}

		min.set_height(
		        std::max(min.height(), object->minimal_size().height() + h_plus));
		preferred.set_height(
		        std::max(preferred.height(),
		                object->preferred_size().height() + h_plus));

		if (current_size.width() < preferred.width()) {
			current_size.set_width(preferred.width());
		}
		if (current_size.height() < preferred.height()) {
			current_size.set_height(preferred.height());
		}

		items_ref().push_back(object);

		set_preferred_size(preferred);
		set_minimal_size(min);

		if(object->expand_x()) m_expandable_items.insert(object);
		else m_fixed_items.insert(object);

		if(! (current_size == size()))
			resize_priv(this, current_size);	// call make_layout() through this function
		else
			make_layout();

		std::cout << "height: " << size().height() << std::endl;

		activate_events();
		object->activate_events();

		bind(object);
		set_in_layout(object, true);
	}

	void HorizontalLayout::add_item (AbstractLayout* layout)
	{

	}

	void HorizontalLayout::remove_item (AbstractForm* object)
	{
		std::vector<AbstractForm*>::iterator it;
		for(it = items_ref().begin(); it != items_ref().end();)
		{
			if ((*it) == object) {
				it = items_ref().erase(it);
			} else {
				it++;
			}
		}

		Size current_size = m_size;
		Size min_size;
		Size preferred_size;

		get_size_hint(true, true, 0, &min_size, &preferred_size);

		m_preferred_size = preferred_size;
		m_minimal_size = min_size;

		set_in_layout(object, false);
		unbind(object);
	}

	void HorizontalLayout::make_layout ()
	{
		if(size().width() == preferred_size().width()) {
			// layout along x with preferred size
			distribute_with_preferred_size();
		} else if (size().width() < preferred_size().width()) {
			// layout along x with small size
			distribute_with_small_size();
		} else {
			// layout along x with large size
			distribute_with_large_size();
		}

//		if(size().height() == preferred_size().height()) {
//			// layout along y with preferred size
//			align_with_preferred_size();
//		} else if (size().height() < preferred_size().height()) {
//			// layout along y with small size
//			align();
//		} else {
			// layout along y with large size
			align();
//		}
	}

	void HorizontalLayout::distribute_with_preferred_size()
	{
		int x = position().x() + margin().left();

		std::vector<AbstractForm*>::iterator it;
		AbstractForm* child = 0;
		for(it = items_ref().begin(); it != items_ref().end(); it++)
		{
			if(! (it == items_ref().begin()))
				x += space();

			child = *it;
			resize_priv(child, child->preferred_size().width(), child->size().height());
			set_pos_priv(child, x, child->position().y());
			x += child->size().width();
		}
	}

	void HorizontalLayout::distribute_with_small_size()
	{
		unsigned int min_exp_w = minimal_expandable_width();
		unsigned int fixed_w = fixed_width();

		unsigned int current_width = size().width();

		unsigned int w_plus = margin().left() + margin().right();

		std::vector<AbstractForm*>::iterator it;
		AbstractForm* child = 0;
		int x = position().x() + margin().left();

		if((current_width - w_plus) >=
				(min_exp_w + fixed_w + (items_ref().size() - 1) * space())) {

			unsigned int single_width = current_width - w_plus - fixed_w - (items_ref().size() - 1) * space();
			single_width = single_width / m_expandable_items.size();

			for(it = items_ref().begin(); it != items_ref().end(); it++)
			{
				if(! (it == items_ref().begin()))
					x += space();

				child = *it;

				if(m_expandable_items.count(child)) {
					resize_priv(child, single_width, child->size().height());
				} else {
					resize_priv(child, child->preferred_size().width(), child->size().height());
				}

				set_pos_priv(child, x, child->position().y());
				x += child->size().width();
			}

		} else {

			std::set<AbstractForm*> normal_items(m_fixed_items);	// set of unminimized items
			//size_t unminimal_items_size = m_xunexpandable_items.size();

			unsigned int unminimal_width = current_width - w_plus - min_exp_w - (items_ref().size() - 1) * space();
			unsigned int w = unminimal_width / normal_items.size();

			for(it = items_ref().begin(); it != items_ref().end(); it++)
			{
				if(! (it == items_ref().begin()))
					x += space();

				child = *it;

				if(m_expandable_items.count(child)) {
					resize_priv(child, child->minimal_size().width(), child->size().height());
				} else {

					if(w < child->minimal_size().width()) {
						resize_priv(child, child->minimal_size().width(), child->size().height());
						normal_items.erase(child);
						unminimal_width = unminimal_width - child->minimal_size().width();
						w = unminimal_width / normal_items.size();
						reset_width_of_fixed_items(&normal_items, w);
					} else {
						resize_priv(child, w, child->size().height());
					}

				}

				set_pos_priv(child, x, child->position().y());
				x += child->size().width();
			}

		}
	}

	void HorizontalLayout::distribute_with_large_size()
	{
		unsigned int fixed_w = fixed_width();

		unsigned int current_width = size().width();

		unsigned int w_plus = margin().left() + margin().right();

		std::vector<AbstractForm*>::iterator it;
		AbstractForm* child = 0;
		int x = position().x() + margin().left();

		unsigned int single_width = current_width - w_plus - fixed_w - (items_ref().size() - 1) * space();

		if(m_expandable_items.size())
			single_width = single_width / m_expandable_items.size();

		for(it = items_ref().begin(); it != items_ref().end(); it++)
		{
			if (!(it == items_ref().begin()))
				x += space();

			child = *it;

			if (m_expandable_items.count(child)) {
				resize_priv(child, single_width, child->size().height());
			} else {
				resize_priv(child, child->preferred_size().width(),
				        child->size().height());
			}

			set_pos_priv(child, x, child->position().y());
			x += child->size().width();
		}

	}

	void HorizontalLayout::align_with_preferred_size()
	{
		int y = position().y() + margin().bottom();

		std::vector<AbstractForm*>::iterator it;
		AbstractForm* child = 0;
		for(it = items_ref().begin(); it != items_ref().end(); it++)
		{
			child = *it;

			if (child->expand_y()) {
				resize_priv(child, child->size().width(), preferred_size().height());
			}

			if (alignment() & AlignTop) {
				set_pos_priv(child, child->position().x(),
						y + (size().height() - child->size().height()));
			} else if (alignment() & AlignBottom) {
				set_pos_priv(child, child->position().x(), y);
			} else if (alignment() & AlignHorizontalCenter) {
				set_pos_priv(child, child->position().x(),
						y + (size().height() - child->size().height()) / 2);
			}
		}
	}

	void HorizontalLayout::align()
	{
		int y = position().y() + margin().bottom();

		unsigned int h = size().height() - margin().top() - margin().bottom();

		std::vector<AbstractForm*>::iterator it;
		AbstractForm* child = 0;
		for(it = items_ref().begin(); it != items_ref().end(); it++)
		{
			child = *it;

			if (child->expand_y() ||
					(child->size().height() > h)) {
				resize_priv(child, child->size().width(), h);
			} else {

				if (alignment() & AlignTop) {
					set_pos_priv(child, child->position().x(),
					        y + (h - child->size().height()));
				} else if (alignment() & AlignBottom) {
					set_pos_priv(child, child->position().x(), y);
				} else if (alignment() & AlignHorizontalCenter) {
					set_pos_priv(child, child->position().x(),
					        y + (h - child->size().height()) / 2);
				}

			}
		}
	}

	void HorizontalLayout::reset_width_of_fixed_items(
			std::set<AbstractForm*>* items, unsigned int width)
	{
		std::set<AbstractForm*>::iterator it;
		AbstractForm* child = 0;

		for(it = items->begin(); it != items->end(); it++)
		{
			child = *it;
			resize_priv(child, width, child->size().height());
		}
	}

	unsigned int HorizontalLayout::minimal_expandable_width()
	{
		unsigned int width = 0;

		std::set<AbstractForm*>::iterator it;
		for(it = m_expandable_items.begin(); it != m_expandable_items.end(); it++)
		{
			width += (*it)->minimal_size().width();
		}

		return width;
	}

	unsigned int HorizontalLayout::fixed_width()
	{
		unsigned int width = 0;

		std::set<AbstractForm*>::iterator it;
		for(it = m_fixed_items.begin(); it != m_fixed_items.end(); it++)
		{
			width += (*it)->size().width();
		}

		return width;

	}

	void HorizontalLayout::get_size_hint (bool count_margin,
										  bool count_space,
										  Size* size,
										  Size* min,
										  Size* preferred)
	{
		Size size_out;
		Size min_size_out;
		Size preferred_size_out;

		AbstractForm* child;
		std::vector<AbstractForm*>::iterator it;

		if(count_margin) {
			size_out.set_width(margin().left());
			min_size_out.set_width(margin().left());
			preferred_size_out.set_width(margin().left());
		}

		for(it = items_ref().begin(); it != items_ref().end(); it++)
		{
			child = *it;
			size_out.add_width(child->size().width());
			size_out.set_height(std::max(size_out.height(),
					child->size().height()));
			min_size_out.add_width(child->minimal_size().width());
			min_size_out.set_height(std::max(min_size_out.height(),
					child->minimal_size().height()));
			preferred_size_out.add_width(child->preferred_size().width());
			preferred_size_out.set_height(std::max(preferred_size_out.height(),
					child->size().height()));
		}

		if(count_margin) {
			size_out.add_width(margin().right());
			size_out.add_height(margin().top() + margin().bottom());

			min_size_out.add_width(margin().right());
			min_size_out.add_height(margin().top() + margin().bottom());

			preferred_size_out.add_width(margin().right());
			preferred_size_out.add_height(margin().top() + margin().bottom());
		}

		if(count_space) {
			size_out.add_width((items_ref().size() - 1) * space());

			min_size_out.add_width((items_ref().size() - 1) * space());

			preferred_size_out.add_width((items_ref().size() - 1) * space());
		}

		if(size) *size = size_out;
		if(min) *min = min_size_out;
		if(preferred) *preferred = preferred_size_out;
	}

	void HorizontalLayout::align_along_x (unsigned int height)
	{
		AbstractForm* child = 0;
		std::vector<AbstractForm*>::iterator it;

		m_expand_x = false;
		m_expand_y = false;

		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			child = *it;

			if (child->expand_y()) {
				resize_priv(child, child->size().width(), height);
			}

			if (alignment() & AlignTop) {
				set_pos_priv(child, child->position().x(),
				        position().y() + margin().bottom()
				                + (height - child->size().height()));
			} else if (alignment() & AlignBottom) {
//				set_pos_priv(child, child->pos().x(),
//						child->pos().y() + margin().bottom());
			} else if (alignment() & AlignHorizontalCenter) {
				set_pos_priv(child, child->position().x(),
				        position().y() + margin().bottom()
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

		std::vector<AbstractForm*>::iterator it;

		for(it = items_ref().begin(); it != items_ref().end(); it++)
		{
			size.add_width((*it)->size().width());
			size.set_height(std::max((*it)->size().height(), size.height()));
		}

		size.add_width(space() * (items_ref().size() - 1) + margin().left() + margin().right());
		size.add_height(margin().top() + margin().bottom());

		return size;
	}

}

