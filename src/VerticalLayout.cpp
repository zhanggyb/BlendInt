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
		change_layout(&m_size);
	}

	/*
	bool VerticalLayout::update (int type, const void* property)
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
				}

				return true;
			}

			default:
				return AbstractLayout::update(type, property);
		}
	}
	*/

	void VerticalLayout::render ()
	{

		std::vector<AbstractForm*>::const_iterator it;
		AbstractForm *item = 0;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
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
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void VerticalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void VerticalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<AbstractForm*>::iterator it;
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
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

		for (it = items_ref().rbegin(); it != items_ref().rend(); it++) {
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
		        - margin().bottom() - (items_ref().size() - 1) * space()
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

		for (it = items_ref().rbegin(); it != items_ref().rend(); it++) {
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
		for (it = items_ref().rbegin(); it != items_ref().rend(); it++) {
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

		for (it = items_ref().rbegin(); it != items_ref().rend(); it++) {
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

		for(size_t i = 0; i < items_ref().size(); i++)
		{
			child = items_ref()[i];
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

			if(i != (items_ref().size() - 1))
				minimal_size.add_height(space());
		}
		minimal_size.add_height(margin().bottom());
		minimal_size.add_width(margin().left() + margin().right());

		return minimal_size;
	}

	void VerticalLayout::add_item (Widget* object)
	{
		unsigned int inner_width = m_size.width() - margin().left()
		        - margin().right();
		unsigned int inner_height = m_size.height() - margin().top()
		        - margin().bottom();

		inner_width = std::max(inner_width, object->size().width());
		inner_height = inner_height + object->size().height();

		if (items_ref().size() == 0) {
			set_pos_priv(object, position().x() + margin().left(),
			        position().y() + margin().bottom());
			m_size.set_height(margin().top() + inner_height + margin().bottom());
			m_minimal_size.add_height(object->minimal_size().height());
		} else {
			set_pos_priv(object,
			        position().x() + margin().left(),
			        position().y() - (object->size().height() + space()));
			m_size.set_height(
			        margin().top() + inner_height + space() + margin().bottom());
			m_minimal_size.add_height(object->minimal_size().height() + space());
		}

		m_size.set_width(margin().left() + inner_width + margin().right());
		m_minimal_size.set_width(std::max(m_minimal_size.width(), object->minimal_size().width()));

		if(!expand_x())
			set_expand_x(object->expand_x());

		if(!expand_y())
			set_expand_y(object->expand_y());

		items_ref().push_back(object);
		bind(object);

		align_along_y(inner_width);

	}

	void VerticalLayout::add_item (AbstractLayout* layout)
	{

	}

	void VerticalLayout::remove_item(AbstractForm * object)
	{

	}

	void VerticalLayout::align_along_y (unsigned int width)
	{
		AbstractForm* child = 0;
		std::vector<AbstractForm*>::iterator it;

		int y = position().y() + m_size.height() - margin().top();
		for (it = items_ref().begin(); it != items_ref().end(); it++) {
			child = *it;

			if(it == items_ref().begin())
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

	Size VerticalLayout::recount_size()
	{
		Size size;

		std::vector<AbstractForm*>::iterator it;

		for(it = items_ref().begin(); it != items_ref().end(); it++)
		{
			size.set_width(std::max((*it)->size().width(), size.width()));
			size.add_height((*it)->size().height());
		}

		size.add_height(space() * (items_ref().size() - 1) + margin().top() + margin().bottom());
		size.add_width(margin().left() + margin().right());

		return size;
	}

}
