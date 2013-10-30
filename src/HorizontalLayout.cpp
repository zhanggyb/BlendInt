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
				change_layout(&m_size);
				return;
			}

			case LayoutPropertyItem: {

				generate_default_layout();

				return;
			}

			default:
				break;
		}
	}

	/*
	bool HorizontalLayout::update (int type, const void* property)
	{
		switch (type) {

			case FormPropertySize: {
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
	*/

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

	void HorizontalLayout::change_layout (const Size* size)
	{
		std::queue<AbstractForm*> expandable_objects;
		std::queue<AbstractForm*> unexpandable_objects;

		AbstractForm* child = 0;

		// first, classify objects in layout according to "hexpand" property
		int fixed_width = 0;
		unsigned int total_height = size->height();
		unsigned int max_widget_height = total_height - margin().top()
		        - margin().bottom();

		for (size_t i = 0; i < items_ref().size(); i++) {
			child = items_ref()[i];
			if (child->expand_x()) {
				expandable_objects.push(child);
			} else {
				unexpandable_objects.push(child);
				fixed_width += child->size().width();
			}
//			total_height = std::max(total_height,
//			        margin().top() + child->size().height()
//			                + margin().bottom());
//			max_widget_height = std::max(max_widget_height,
//			        child->size().height());
		}

		// average the width of each expandable object along horizontal
		if (expandable_objects.size() > 0) {
			int flexible_width = size->width() - margin().left()
			        - margin().right() - (items_ref().size() - 1) * space()
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
		Point pos = position();
		pos.set_x(pos.x() + margin().left());
		pos.set_y(pos.y() + margin().bottom());

		for (size_t i = 0; i < items_ref().size(); i++) {
			child = items_ref()[i];

			// set position
			set_pos_priv(child, pos);

			// set height
			if (child->expand_y()) {
				resize_priv(child, child->size().width(), max_widget_height);
			} else {
				if (alignment() & AlignTop) {
					set_pos_priv(child, child->position().x(),
					        position().y()
					                + (total_height
					                        - (margin().top()
					                                + child->size().height())));
				} else if (alignment() & AlignBottom) {
					set_pos_priv(child, child->position().x(),
					        position().y() + margin().bottom());
				} else if (alignment() & AlignHorizontalCenter) {
					set_pos_priv(child, child->position().x(),
					        position().y() + margin().bottom()
					                + (max_widget_height
					                        - child->size().height()) / 2);
				}
			}

			pos.set_x(pos.x() + child->size().width() + space());
		}

		return;
	}

	void HorizontalLayout::generate_default_layout ()
	{
		unsigned int total_width = 0;
		unsigned int total_height = m_size.height();
		unsigned int max_widget_height = 0;

		AbstractForm* child = 0;
		total_width = margin().left();
		for (size_t i = 0; i < items_ref().size(); i++) {
			child = items_ref()[i];
			set_pos_priv(child, position().x() + total_width,
			        position().y() + margin().bottom());
			total_width = total_width + child->size().width();
			total_height = std::max(total_height,
			        margin().top() + child->size().height()
			                + margin().bottom());
			max_widget_height = std::max(max_widget_height,
			        child->size().height());

			if (i != (items_ref().size() - 1))
				total_width += space();
		}
		total_width = total_width + margin().right();

		for (size_t i = 0; i < items_ref().size(); i++) {
			child = items_ref()[i];

			if (child->expand_y()) {
				resize_priv(child, child->size().width(), max_widget_height);
			} else {
				if (alignment() & AlignTop) {
					set_pos_priv(child, child->position().x(),
					        position().y()
					                + (total_height
					                        - (margin().top()
					                                + child->size().height())));
				} else if (alignment() & AlignBottom) {
					// TODO: not needed as already done in previous loop
					set_pos_priv(child, child->position().x(),
					        position().y() + margin().bottom());
				} else if (alignment() & AlignHorizontalCenter) {
					set_pos_priv(child, child->position().x(),
					        position().y()
					                + (total_height - child->size().height())
					                        / 2);
				}
			}

		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);
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

	void HorizontalLayout::add_item (Widget* object)
	{
		bind(object);
		set_in_layout(object, true);

		if (sizing_mode() == LayoutFlow) {
			items_ref().push_back(object);

			Size layout_size;
			Size layout_preferred_size;
			Size layout_min_size;

			get_size_hint(true, true, &layout_size, &layout_min_size, &layout_preferred_size);

			std::cout << "size before: " << size().width() << " " << size().height() << std::endl;

			if(layout_preferred_size.width() < size().width()) {
				layout_size.set_width(size().width());
			} else {
				layout_size.set_width(layout_preferred_size.width());
			}

			if(layout_preferred_size.height() < size().height()) {
				layout_size.set_height(size().height());
			} else {
				layout_size.set_height(layout_preferred_size.height());
			}

			change_layout(&layout_size);
			m_preferred_size = layout_preferred_size;
			m_minimal_size = layout_min_size;

			resize(layout_size);

			std::cout << "size: " << size().width() << " " << size().height() << std::endl;

		} else {	// LayoutFixed
			items_ref().push_back(object);
			Size size = m_size;
			change_layout(&size);
		}
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

		change_layout(&current_size);

		m_preferred_size = preferred_size;
		m_minimal_size = min_size;

		set_in_layout(object, false);
		unbind(object);
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

